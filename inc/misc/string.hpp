#ifndef MODERN_CPP_INC_MISC_STRING_HPP
#define MODERN_CPP_INC_MISC_STRING_HPP

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace mcpp {

template <typename T> class BasicString {
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

private:
  explicit BasicString(std::size_t);

  [[nodiscard]] std::size_t strLen_(const T *) const;
  void resize_(std::size_t);

  static constexpr const char *const outOfRangeMsg_ = "out of range";

  T *data_;
  std::size_t size_;
};

using String = BasicString<char>;

} // namespace mcpp

template <typename T>
inline mcpp::BasicString<T>::BasicString() : data_(nullptr), size_(0) {}

template <typename T>
mcpp::BasicString<T>::BasicString(const BasicString &other)
    : size_(other.size_) {
  data_ = new T[size_];
  std::copy(other.data_, other.data_ + size_, data_);
}

template <typename T>
inline mcpp::BasicString<T>::BasicString(BasicString &&other) noexcept
    : data_(other.data_), size_(other.size_) {
  other.data_ = nullptr;
  other.size_ = 0;
}

template <typename T> mcpp::BasicString<T>::BasicString(const T *other) {
  data_ = new T[size_ = strLen_(other)];
  std::copy(other, other + size_, data_);
}

template <typename T> inline mcpp::BasicString<T>::~BasicString() {
  delete[] data_;
  size_ = 0;
}

template <typename T>
mcpp::BasicString<T> &
mcpp::BasicString<T>::operator=(const BasicString &other) {
  if (this == &other)
    goto skipCopy;
  if (other.size_ != size_) // could be optimized for time by resizing only if
                            // other.size_ is greater, but would use more memory
    resize_(size_ = other.size_);
  std::copy(other.data_, other.data_ + size_, data_);
skipCopy:
  return *this;
}

template <typename T>
inline mcpp::BasicString<T> &
mcpp::BasicString<T>::operator=(BasicString &&other) noexcept {
  if (this == &other)
    goto skipMove;
  delete[] data_;
  data_ = other.data_;
  size_ = other.size_;
  other.data_ = nullptr;
  other.size_ = 0;
skipMove:
  return *this;
}

template <typename T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator=(const T *other) {
  auto otherLength = strLen_(other);
  if (size_ != otherLength)
    resize_(otherLength);
  std::copy(other, other + size_, data_);
  return *this;
}

template <typename T>
inline bool mcpp::BasicString<T>::operator==(const BasicString &other) const {
  return std::equal(data_, data_ + size_, other.data_,
                    other.data_ + other.size_);
}

template <typename T>
inline bool mcpp::BasicString<T>::operator==(const T *other) const {
  return std::equal(data_, data_ + size_, other, other + strLen_(other));
}

template <typename T>
inline bool mcpp::BasicString<T>::operator!=(const BasicString &other) const {
  return !operator==(other);
}

template <typename T>
inline bool mcpp::BasicString<T>::operator!=(const T *other) const {
  return !operator==(other);
}

template <typename T>
mcpp::BasicString<T>
mcpp::BasicString<T>::operator+(const BasicString &other) const {
  BasicString result(size_ + other.size_);
  auto midpoint = std::copy(data_, data_ + size_, result.data_);
  std::copy(other.data_, other.data_ + other.size_, midpoint);
  return result;
}

template <typename T>
mcpp::BasicString<T> mcpp::BasicString<T>::operator+(const T *other) const {
  return *this + BasicString(other);
}

template <typename T>
mcpp::BasicString<T> mcpp::BasicString<T>::operator+(const T &elem) const {
  BasicString result(size_ + 1);
  std::copy(data_, data_ + size_, result.data_);
  result[size_] = elem;
  return result;
}

template <typename T>
mcpp::BasicString<T> &
mcpp::BasicString<T>::operator+=(const BasicString &other) {
  return *this = *this + other;
}

template <typename T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator+=(const T *other) {
  return *this = *this + other;
}

template <typename T>
mcpp::BasicString<T> &mcpp::BasicString<T>::operator+=(const T &elem) {
  return *this = *this + elem;
}

template <typename T>
inline const T &mcpp::BasicString<T>::operator[](std::size_t index) const {
  if (index >= size_)
    throw std::out_of_range(outOfRangeMsg_);
  return data_[index];
}

template <typename T>
inline T &mcpp::BasicString<T>::operator[](std::size_t index) {
  if (index >= size_)
    throw std::out_of_range(outOfRangeMsg_);
  return data_[index];
}

template <typename T> inline std::size_t mcpp::BasicString<T>::length() const {
  return size_;
}

template <typename T>
mcpp::BasicString<T>::BasicString(std::size_t initialCapacity)
    : data_(new T[initialCapacity]), size_(initialCapacity) {}

template <typename T>
std::size_t mcpp::BasicString<T>::strLen_(const T *str) const {
  auto p = str;
  for (; *p != 0; ++p)
    ;
  return p - str;
}

template <typename T> void mcpp::BasicString<T>::resize_(std::size_t newSize) {
  delete[] data_;
  data_ = new T[size_ = newSize];
}

#endif // MODERN_CPP_INC_MISC_STRING_HPP
