#ifndef MODERN_CPP_INC_DATA_STRUCTURES_ARRAY_HPP
#define MODERN_CPP_INC_DATA_STRUCTURES_ARRAY_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace mcpp::data_structures {

template <typename T> class Array {
public:
  using Iterator = T *;
  using ConstIterator = const T *;

  Array();
  explicit Array(std::size_t);
  Array(const std::initializer_list<T> &);

  ~Array();

  auto push(const T &);
  auto remove(const T &);
  [[nodiscard]] auto contains(const T &) const;
  auto clear();
  [[nodiscard]] auto size() const;

  [[nodiscard]] const auto &operator[](std::size_t) const;
  auto &operator[](std::size_t);

  Iterator begin();
  Iterator end();

  // can't use auto bc of const correctness apparently
  ConstIterator begin() const;
  ConstIterator end() const;

private:
  auto expand_();
  auto reallocate_(std::size_t);

  static constexpr auto outOfRangeMsg_ = "out of range";
  static constexpr auto initialCapacity_ = std::size_t(10);
  // reason for this factor here: https://archive.ph/Z2R8w
  static constexpr auto expansionFactor_ = 1.618033988749894;

  T *data_;
  std::size_t capacity_, size_;
};

template <typename T> Array<T>::Array() : Array(initialCapacity_) {}

template <typename T>
Array<T>::Array(std::size_t initialCapacity)
    : data_(new T[initialCapacity]), capacity_(initialCapacity), size_(0) {}

template <typename T>
Array<T>::Array(const std::initializer_list<T> &list)
    : data_(new T[list.size()]), capacity_(list.size()), size_(list.size()) {
  std::copy(std::cbegin(list), std::cend(list), data_);
}

template <typename T> Array<T>::~Array() { delete[] data_; }

template <typename T> auto Array<T>::push(const T &value) {
  if (size_ == capacity_)
    expand_();
  data_[size_++] = value;
}

template <typename T> auto Array<T>::remove(const T &value) {
  if (std::remove(data_, data_ + size_, value) < end()) {
    --size_;
    return true;
  }
  return false;
}

template <typename T> auto Array<T>::contains(const T &value) const {
  const auto end = data_ + size_;
  return std::find(data_, end, value) != end;
}

template <typename T> auto Array<T>::clear() {
  reallocate_(initialCapacity_);
  size_ = 0;
}

template <typename T> auto Array<T>::size() const { return size_; }

template <typename T>
const auto &Array<T>::operator[](std::size_t index) const {
  // if (index >= size_)
  // throw std::out_of_range(outOfRangeMsg_);
  // return data_[index];
  return const_cast<const Array *>(this).operator[](index); // ? valid i guess
}

template <typename T> auto &Array<T>::operator[](std::size_t index) {
  if (index >= size_)
    throw std::out_of_range(outOfRangeMsg_);
  return data_[index];
}

template <typename T> Array<T>::Iterator Array<T>::begin() { return data_; }

template <typename T> Array<T>::Iterator Array<T>::end() {
  return data_ + size_;
}

template <typename T> Array<T>::ConstIterator Array<T>::begin() const {
  return data_;
}

template <typename T> Array<T>::ConstIterator Array<T>::end() const {
  return data_ + size_;
}

template <typename T> auto Array<T>::reallocate_(std::size_t newCapacity) {
  auto newData = new T[capacity_ = newCapacity];
  std::copy(data_, data_ + size_, newData);
  delete[] data_;
  data_ = newData;
}

template <typename T> auto Array<T>::expand_() {
  reallocate_(capacity_ * expansionFactor_);
}

// non-member functions

template <typename T>
std::ostream &operator<<(std::ostream &stream, const Array<T> &array) {
  stream << '[';
  std::copy(std::cbegin(array), std::cend(array),
            std::ostream_iterator<T>(stream, ","));
  if (array.size() != 0)
    stream << '\b';
  return stream << ']';
}

} // namespace data_structures

#endif // MODERN_CPP_INC_DATA_STRUCTURES_ARRAY_HPP
