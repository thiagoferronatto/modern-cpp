#ifndef MODERN_CPP_INC_MISC_STRING_HPP
#define MODERN_CPP_INC_MISC_STRING_HPP

#include "type_traits/type_traits.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace mcpp {

template <typename T>
concept Char = type_traits::IsCharV<T>;

template <Char T> class BasicString {
public:
  BasicString();
  BasicString(const BasicString &);
  BasicString(BasicString &&) noexcept;
  BasicString(const T *);

  ~BasicString();

  BasicString &operator=(const BasicString &);
  BasicString &operator=(BasicString &&) noexcept;
  BasicString &operator=(const T *);

  [[nodiscard]] bool operator==(const BasicString &) const;
  [[nodiscard]] bool operator==(const T *) const;
  [[nodiscard]] bool operator!=(const BasicString &) const;
  [[nodiscard]] bool operator!=(const T *) const;

  [[nodiscard]] BasicString operator+(const BasicString &) const;
  [[nodiscard]] BasicString operator+(const T *) const;
  [[nodiscard]] BasicString operator+(const T &) const;

  BasicString &operator+=(const BasicString &);
  BasicString &operator+=(const T *);
  BasicString &operator+=(const T &);

  [[nodiscard]] const T &operator[](std::size_t) const;
  T &operator[](std::size_t);

  [[nodiscard]] std::size_t length() const;

  friend std::ostream &operator<<(std::ostream &os, const BasicString &str) {
    std::copy(str.data_, str.data_ + str.size_, std::ostream_iterator<T>(os));
    return os;
  }

  // TODO: maybe someday add an operator>> to insert values from istream

private:
  explicit BasicString(std::size_t);

  [[nodiscard]] std::size_t strLen_(const T *) const;
  void resize_(std::size_t);

  static constexpr const char *const outOfRangeMsg_ = "out of range";
  static constexpr std::size_t ssoBufSize_ = 16;

  T *data_;
  std::size_t size_;
  T buf_[ssoBufSize_];
};

using String = BasicString<char>;

} // namespace mcpp

template <mcpp::Char T>
inline mcpp::BasicString<T>::BasicString() : data_(buf_), size_(0) {}

template <mcpp::Char T>
mcpp::BasicString<T>::BasicString(const BasicString &other)
    : data_(buf_), size_(other.size_) {
  if (other.size_ > ssoBufSize_)
    data_ = new T[size_];
  std::copy(other.data_, other.data_ + size_, data_);
}

template <mcpp::Char T>
inline mcpp::BasicString<T>::BasicString(BasicString &&other) noexcept
    : data_(buf_), size_(other.size_) {
  if (other.size_ > 16) {
    data_ = other.data_;
    other.data_ = nullptr;
  } else
    std::copy(other.data_, other.data_ + size_, buf_);
  other.size_ = 0;
}

template <mcpp::Char T> mcpp::BasicString<T>::BasicString(const T *other) {
  size_ = strLen_(other);
  data_ = size_ > 16 ? new T[size_] : buf_;
  std::copy(other, other + size_, data_);
}

template <mcpp::Char T> inline mcpp::BasicString<T>::~BasicString() {
  if (data_ != buf_)
    delete[] data_;
  size_ = 0;
}

template <mcpp::Char T>
mcpp::BasicString<T> &
mcpp::BasicString<T>::operator=(const BasicString &other) {
  if (this == &other)
    goto skipCopy;
  if (other.size_ > ssoBufSize_) {
    // this resizes the array even if other.size_ == size_, but the additional
    // ifs I'd have to write to optimize for that really specific case make me
    // want to take my name off the census
    if (data_ != buf_)
      delete[] data_;
    data_ = new T[other.size_];
  } else if (data_ != buf_) {
    delete[] data_;
    data_ = buf_;
  }
  std::copy(other.data_, other.data_ + other.size_, data_);
  size_ = other.size_;
skipCopy:
  return *this;
}

template <mcpp::Char T>
inline mcpp::BasicString<T> &
mcpp::BasicString<T>::operator=(BasicString &&other) noexcept {
  if (this == &other)
    goto skipMove;
  if (other.size_ > ssoBufSize_) {
    if (data_ != buf_)
      delete[] data_;
    data_ = other.data_;
  } else {
    if (data_ != buf_) {
      delete[] data_;
      data_ = buf_;
    }
    std::copy(other.data_, other.data_ + other.size_, data_); // tiny copy
  }
  size_ = other.size_;
  other.data_ = nullptr;
  other.size_ = 0;
skipMove:
  return *this;
}

template <mcpp::Char T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator=(const T *other) {
  auto otherLength = strLen_(other);
  if (otherLength > ssoBufSize_) {
    if (data_ != buf_)
      delete[] data_;
    data_ = new T[otherLength];
  } else if (data_ != buf_) {
    delete[] data_;
    data_ = buf_;
  }
  std::copy(other, other + otherLength, data_);
  size_ = otherLength;
  return *this;
}

template <mcpp::Char T>
inline bool mcpp::BasicString<T>::operator==(const BasicString &other) const {
  return std::equal(data_, data_ + size_, other.data_,
                    other.data_ + other.size_);
}

template <mcpp::Char T>
inline bool mcpp::BasicString<T>::operator==(const T *other) const {
  return std::equal(data_, data_ + size_, other, other + strLen_(other));
}

template <mcpp::Char T>
inline bool mcpp::BasicString<T>::operator!=(const BasicString &other) const {
  return !operator==(other);
}

template <mcpp::Char T>
inline bool mcpp::BasicString<T>::operator!=(const T *other) const {
  return !operator==(other);
}

template <mcpp::Char T>
mcpp::BasicString<T>
mcpp::BasicString<T>::operator+(const BasicString &other) const {
  BasicString result(size_ + other.size_);
  auto midpoint = std::copy(data_, data_ + size_, result.data_);
  std::copy(other.data_, other.data_ + other.size_, midpoint);
  return result;
}

template <mcpp::Char T>
mcpp::BasicString<T> mcpp::BasicString<T>::operator+(const T *other) const {
  return *this + BasicString(other);
}

template <mcpp::Char T>
mcpp::BasicString<T> mcpp::BasicString<T>::operator+(const T &elem) const {
  BasicString result(size_ + 1);
  std::copy(data_, data_ + size_, result.data_);
  result[size_] = elem;
  return result;
}

template <mcpp::Char T>
mcpp::BasicString<T> &
mcpp::BasicString<T>::operator+=(const BasicString &other) {
  return *this = *this + other;
}

template <mcpp::Char T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator+=(const T *other) {
  return *this = *this + other;
}

template <mcpp::Char T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator+=(const T &elem) {
  return *this = *this + elem;
}

template <mcpp::Char T>
inline const T &mcpp::BasicString<T>::operator[](std::size_t index) const {
  if (index >= size_)
    throw std::out_of_range(outOfRangeMsg_);
  return data_[index];
}

template <mcpp::Char T>
inline T &mcpp::BasicString<T>::operator[](std::size_t index) {
  if (index >= size_)
    throw std::out_of_range(outOfRangeMsg_);
  return data_[index];
}

template <mcpp::Char T>
inline std::size_t mcpp::BasicString<T>::length() const {
  return size_;
}

template <mcpp::Char T>
mcpp::BasicString<T>::BasicString(std::size_t initialCapacity)
    : data_(new T[initialCapacity]), size_(initialCapacity) {}

template <mcpp::Char T>
std::size_t mcpp::BasicString<T>::strLen_(const T *str) const {
  auto p = str;
  for (; *p != 0; ++p)
    ;
  return p - str;
}

template <mcpp::Char T>
void mcpp::BasicString<T>::resize_(std::size_t newSize) {
  delete[] data_;
  data_ = new T[size_ = newSize];
}

#endif // MODERN_CPP_INC_MISC_STRING_HPP
