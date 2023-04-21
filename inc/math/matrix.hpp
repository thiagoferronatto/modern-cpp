#ifndef MODERN_CPP_INC_MATH_MATRIX_HPP
#define MODERN_CPP_INC_MATH_MATRIX_HPP

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdio>
#include <initializer_list>
#include <numeric>
#include <type_traits>

namespace mcpp::math {

template <std::floating_point T, std::size_t width_, std::size_t height_>
class Matrix {
  using MatrixInitList = std::initializer_list<std::initializer_list<T>>;

public:
  template <std::size_t w = width_, std::size_t h = height_>
  static typename std::enable_if_t<w == h, Matrix> identity() {
    Matrix result;
    for (std::size_t i = 0; i < w; ++i)
      result(i, i) = 1;
    return result;
  }

  Matrix() = default;

  Matrix(const Matrix &other) : Matrix(1) {
    std::copy_n(other.data_, width_ * height_, data_);
  }

  Matrix(Matrix &&other) noexcept : data_(other.data_) {
    other.data_ = nullptr;
  }

  Matrix(const MatrixInitList &initList) {
    // I'd love to do this with a static_assert, if only there was a way
    assert(initList.size() == height_ && initList.begin()->size() == width_);
    for (std::size_t i = 0; i < initList.size(); ++i)
      for (std::size_t j = 0; j < (initList.begin() + i)->size(); ++j)
        operator()(i, j) = *((initList.begin() + i)->begin() + j);
  }

  ~Matrix() { delete[] data_; }

  Matrix &operator=(const Matrix &other) {
    if (this == &other)
      goto skipCopy;
    std::copy_n(other.data_, width_ * height_, data_);
  skipCopy:
    return *this;
  }

  Matrix &operator=(Matrix &&other) noexcept {
    if (this == &other)
      goto skipMove;
    data_ = other.data_;
    other.data_ = nullptr;
  skipMove:
    return *this;
  }

  Matrix &operator=(const MatrixInitList &initList) {
    static_assert(initList.size() == height_ && initList[0].size() == width_);
    for (std::size_t i = 0; i < initList.size(); ++i)
      for (std::size_t j = 0; j < initList[i].size(); ++j)
        operator()(i, j) = initList[i][j];
  }

  bool operator==(const Matrix &other) const {
    return std::equal(data_, data_ + width_ * height_, other.data_);
  }

  bool operator!=(const Matrix &other) const { return !operator==(other); }

  Matrix operator-() const { return *this * T(-1); }

  Matrix operator+(const Matrix &other) const {
    Matrix result(1);
    std::transform(data_, data_ + width_ * height_, other.data_, result.data_,
                   std::plus());
    return result;
  }

  Matrix operator-(const Matrix &other) const { return *this + -other; }

  Matrix operator*(T scalar) const {
    Matrix result(1);
    std::transform(data_, data_ + width_ * height_, result.data_,
                   [scalar](T element) { return scalar * element; });
    return result;
  }

  template <std::size_t otherWidth>
  Matrix<T, otherWidth, height_>
  operator*(const Matrix<T, otherWidth, width_> &other) const {
    Matrix<T, otherWidth, height_> result;
    for (std::size_t i = 0; i < height_; ++i)
      for (std::size_t j = 0; j < otherWidth; ++j)
        for (std::size_t k = 0; k < width_; ++k)
          result(i, j) += operator()(i, k) * other(k, j);
    return result;
  }

  T &operator()(std::size_t row, std::size_t col) {
    return data_[row * width_ + col];
  }

  T operator()(std::size_t row, std::size_t col) const {
    return data_[row * width_ + col];
  }

  Matrix<T, height_, width_> transposed() const {
    Matrix<T, height_, width_> result(1);
    for (std::size_t i = 0; i < height_; ++i)
      for (std::size_t j = 0; j < width_; ++j)
        result(j, i) = operator()(i, j);
    return result;
  }

  void transpose() { *this = std::move(transposed()); }

  std::size_t width() const { return width_; }

  std::size_t height() const { return height_; }

  typename std::enable_if_t<width_ == height_, std::size_t> order() const {
    return width_;
  }

private:
  // dummy int, constructs without initializing for performance reasons
  explicit Matrix(int) : data_(new T[width_ * height_]) {}

  T *data_ = new T[width_ * height_]{};
};

template <std::size_t width, std::size_t height>
using FMatrix = Matrix<float, width, height>;

template <std::floating_point T, std::size_t dim>
using Vector = Matrix<T, 1, dim>;

} // namespace mcpp::math

#endif // MODERN_CPP_INC_MATH_MATRIX_HPP
