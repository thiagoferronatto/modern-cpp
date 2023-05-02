#ifndef MODERN_CPP_INC_MATH_MATRIX_HPP
#define MODERN_CPP_INC_MATH_MATRIX_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdio>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <type_traits>

namespace mcpp::math {

// TODO: if constexpr (width_ == 0 || height_ == 0) { /* treat as dynamic */ }
template <std::floating_point T, std::size_t width_, std::size_t height_>
class Matrix {
  using MatrixInitList = std::initializer_list<std::initializer_list<T>>;

public:
  template <std::size_t w = width_, std::size_t h = height_>
  static typename std::enable_if_t<w == h, Matrix> identity();

  Matrix() = default;
  Matrix(const Matrix &);
  Matrix(Matrix &&) noexcept;
  Matrix(const MatrixInitList &);
  // dummy int, constructs without initializing to zero
  explicit Matrix(int);

  ~Matrix();

  Matrix &operator=(const Matrix &);
  Matrix &operator=(Matrix &&) noexcept;
  Matrix &operator=(const MatrixInitList &);

  [[nodiscard]] bool operator==(const Matrix &) const;
  [[nodiscard]] bool operator!=(const Matrix &) const;

  [[nodiscard]] Matrix operator-() const;

  [[nodiscard]] Matrix operator+(const Matrix &) const;
  [[nodiscard]] Matrix operator-(const Matrix &) const;
  [[nodiscard]] Matrix operator*(T) const;
  [[nodiscard]] Matrix operator/(T) const;

  template <std::size_t otherWidth>
  [[nodiscard]] Matrix<T, otherWidth, height_>
  operator*(const Matrix<T, otherWidth, width_> &) const;

  template <std::size_t w = width_, std::size_t h = height_>
  [[nodiscard]] typename std::enable_if_t<w == 1 || h == 1, T>
  dot(const Matrix &) const;

  template <std::size_t w = width_, std::size_t h = height_>
  [[nodiscard]]
  typename std::enable_if_t<(w == 1 && h == 3) || (h == 1 && w == 3),
                            Matrix<T, w, h>>
  cross(const Matrix &) const;

  template <std::size_t w = width_, std::size_t h = height_>
  [[nodiscard]] typename std::enable_if_t<w == 1 || h == 1, T> length() const;

  Matrix &operator+=(const Matrix &) const;
  Matrix &operator-=(const Matrix &) const;
  Matrix &operator*=(T) const;
  Matrix &operator/=(T) const;

  T &operator()(std::size_t, std::size_t);
  [[nodiscard]] T operator()(std::size_t, std::size_t) const;

  template <std::size_t w = width_, std::size_t h = height_>
  [[nodiscard]] std::enable_if_t<w == 1 || h == 1, T>
  operator()(std::size_t) const;

  [[nodiscard]] Matrix<T, height_, width_> transposed() const;
  typename std::enable_if_t<width_ == height_, void> transpose();

  [[nodiscard]] std::size_t width() const;
  [[nodiscard]] std::size_t height() const;

  template <std::size_t w = width_, std::size_t h = height_>
  [[nodiscard]] typename std::enable_if_t<w == h, std::size_t> order() const;

  [[nodiscard]] const T *begin() const;
  [[nodiscard]] const T *end() const;

private:
  T *data_ = new T[width_ * height_]{};

  friend Matrix<T, 0, 0>;
};

// template spec for dynamic matrix (dimensions unknown @ compile time)
template <std::floating_point T> class Matrix<T, 0, 0> {
  using MatrixInitList = std::initializer_list<std::initializer_list<T>>;

public:
  Matrix(std::size_t width, std::size_t height)
      : width_(width), height_(height), data_(new T[width * height]) {}

  template <std::size_t w, std::size_t h>
  explicit Matrix(const Matrix<T, w, h> &other)
      : width_(other.width()), height_(other.height()),
        data_(new T[width_ * height_]) {
    std::copy_n(other.data_, width_ * height_, data_);
  }

  template <std::size_t w, std::size_t h>
  explicit Matrix(Matrix<T, w, h> &&other) noexcept
      : width_(other.width()), height_(other.height()), data_(other.data_) {
    if constexpr (w == 0 || h == 0)
      other.width_ = other.height_ = 0;
    other.data_ = nullptr;
  }

  Matrix(const MatrixInitList &initList)
      : width_(initList.begin()->size()), height_(initList.size()),
        data_(new T[width_ * height_]) {
    for (std::size_t i = 0; i < initList.size(); ++i)
      for (std::size_t j = 0; j < (initList.begin() + i)->size(); ++j)
        operator()(i, j) = *((initList.begin() + i)->begin() + j);
  }

  template <std::size_t w, std::size_t h>
  Matrix &operator=(const Matrix<T, w, h> &other) {
    if (this == &other)
      goto skipCopy;
    if (other.width() * other.height() == width_ * height_)
      goto skipRealloc;
    delete[] data_;
    data_ = new T[other.width() * other.height()];
  skipRealloc:
    width_ = other.width();
    height_ = other.height();
    std::copy_n(other.data_, width_ * height_, data_);
  skipCopy:
    return *this;
  }

  template <std::size_t w, std::size_t h>
  Matrix &operator=(Matrix<T, w, h> &&other) {
    if (this == &other)
      goto skipMove;
    width_ = other.width();
    height_ = other.height();
    data_ = other.data_;
    if constexpr (w == 0 || h == 0)
      other.width_ = other.height_ = 0;
    other.data_ = nullptr;
  skipMove:
    return *this;
  }

  Matrix &operator=(const MatrixInitList &initList) {
    const auto listWidth = initList.begin()->size(),
               listHeight = initList.size();
    if (listWidth * listHeight == width_ * height_)
      goto skipRealloc;
    delete[] data_;
    data_ = new T[listWidth * listHeight];
  skipRealloc:
    width_ = listWidth;
    height_ = listHeight;
    for (std::size_t i = 0; i < listHeight; ++i)
      for (std::size_t j = 0; j < (initList.begin() + i)->size(); ++j)
        operator()(i, j) = *((initList.begin() + i)->begin() + j);
    return *this;
  }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] bool operator==(const Matrix<T, w, h> &other) {
    return std::equal(data_, data_ + width_ * height_, other.data_,
                      other.data_ + other.width() * other.height());
  }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] bool operator!=(const Matrix<T, w, h> &other) {
    return !operator==(other);
  }

  [[nodiscard]] Matrix operator-() const { return *this * T(-1); }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] Matrix operator+(const Matrix<T, w, h> &other) const {
    assert(width_ == other.width() && height_ == other.height());
    Matrix result(other.width(), other.height());
    std::transform(data_, data_ + other.width() * other.height(), other.data_,
                   result.data_, std::plus());
    return result;
  }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] Matrix operator-(const Matrix<T, w, h> &other) const {
    return *this + -other;
  }

  [[nodiscard]] Matrix operator*(T scalar) const {
    Matrix result(width_, height_);
    std::transform(data_, data_ + width_ * height_, result.data_,
                   [scalar](T elem) { return elem * scalar; });
    return result;
  }

  [[nodiscard]] Matrix operator/(T scalar) const {
    return *this * (T(1) / scalar);
  }

  // dynamic*dynamic or dynamic*static
  template <std::size_t w, std::size_t h>
  [[nodiscard]] Matrix operator*(const Matrix<T, w, h> &other) const {
    assert(width_ == other.height());
    Matrix result(other.width(), height_);
    for (std::size_t i = 0; i < height_; ++i)
      for (std::size_t j = 0; j < other.width(); ++j)
        for (std::size_t k = 0; k < width_; ++k)
          result(i, j) += operator()(i, k) * other(k, j);
    return result;
  }

  // static*dynamic
  template <std::size_t w, std::size_t h>
  [[nodiscard]] friend std::enable_if_t<w != 0 && h != 0, Matrix<T, 0, 0>>
  operator*(const Matrix<T, w, h> &a, const Matrix<T, 0, 0> &b) {
    assert(w == b.height_);
    Matrix result(b.width_, h);
    for (std::size_t i = 0; i < h; ++i)
      for (std::size_t j = 0; j < b.width_; ++j)
        for (std::size_t k = 0; k < w; ++k)
          result(i, j) += a(i, k) * b(k, j);
    return result;
  }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] T dot(const Matrix<T, w, h> &other) const {
    assert(width_ == other.width() && height_ == other.height() &&
           (width_ == 1 || height_ == 1));
    return std::inner_product(data_, data_ + w * h, other.data_, T());
  }

  template <std::size_t w, std::size_t h>
  [[nodiscard]] Matrix cross(const Matrix<T, w, h> &other) const {
    assert(width_ == other.width() && height_ == other.height() &&
           ((width_ == 1 && height_ == 3) || (height_ == 1 && width_ == 3)));
    // TODO: implement cross product here too
  }

  template <std::size_t w, std::size_t h>
  Matrix &operator+=(const Matrix<T, w, h> &other);

  template <std::size_t w, std::size_t h>
  Matrix &operator-=(const Matrix<T, w, h> &other);

  template <std::size_t w, std::size_t h> Matrix &operator*=(T scalar);

  template <std::size_t w, std::size_t h> Matrix &operator/=(T scalar);

  template <std::size_t w, std::size_t h>
  Matrix &operator*=(const Matrix<T, w, h> &other);

  [[nodiscard]] T length() const {
    assert(width_ == 1 || height_ == 1);
    return std::sqrt(dot(*this));
  }

  [[nodiscard]] T operator()(std::size_t row, std::size_t column) const {
    assert(row < height_ && column < width_);
    return data_[row * width_ + column];
  }

  T &operator()(std::size_t row, std::size_t column) {
    assert(row < height_ && column < width_);
    return data_[row * width_ + column];
  }

  [[nodiscard]] Matrix transposed() const;
  void transpose();

  [[nodiscard]] std::size_t width() const { return width_; }

  [[nodiscard]] std::size_t height() const { return height_; }

private:
  std::size_t width_, height_;
  T *data_;
};

// aliases

template <std::size_t width, std::size_t height>
using FMatrix = Matrix<float, width, height>;

using FMatrix3x3 [[maybe_unused]] = FMatrix<3, 3>;
using FMatrix4x4 [[maybe_unused]] = FMatrix<4, 4>;

template <std::floating_point T, std::size_t dim>
using Vector = Matrix<T, 1, dim>;

template <std::size_t dim> using FVector = Vector<float, dim>;

using FVector3 [[maybe_unused]] = FVector<3>;
using FVector4 [[maybe_unused]] = FVector<4>;

template <std::floating_point T, std::size_t dim>
using RowVector = Matrix<T, dim, 1>;

template <std::size_t dim> using FRowVector = RowVector<float, dim>;

using FRowVector3 [[maybe_unused]] = FRowVector<3>;
using FRowVector4 [[maybe_unused]] = FRowVector<4>;

template <std::floating_point T> using DMatrix = Matrix<T, 0, 0>;

using DFMatrix [[maybe_unused]] = DMatrix<float>;

// implementation

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
typename std::enable_if_t<w == h, Matrix<T, width_, height_>>
Matrix<T, width_, height_>::identity() {
  Matrix result;
  for (std::size_t i = 0; i < w; ++i)
    result(i, i) = 1;
  return result;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>::Matrix(const Matrix &other) : Matrix(1) {
  std::copy_n(other.data_, width_ * height_, data_);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>::Matrix(Matrix &&other) noexcept
    : data_(other.data_) {
  other.data_ = nullptr;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>::Matrix(const Matrix::MatrixInitList &initList) {
  // I'd love to do this with a static_assert, if only there was a way
  assert(initList.size() == height_ && initList.begin()->size() == width_);
  for (std::size_t i = 0; i < initList.size(); ++i)
    for (std::size_t j = 0; j < (initList.begin() + i)->size(); ++j)
      operator()(i, j) = *((initList.begin() + i)->begin() + j);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>::Matrix(int) : data_(new T[width_ * height_]) {}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>::~Matrix() {
  delete[] data_;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator=(const Matrix &other) {
  if (this == &other)
    goto skipCopy;
  std::copy_n(other.data_, width_ * height_, data_);
skipCopy:
  return *this;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator=(Matrix &&other) noexcept {
  if (this == &other)
    goto skipMove;
  data_ = other.data_;
  other.data_ = nullptr;
skipMove:
  return *this;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator=(const Matrix::MatrixInitList &initList) {
  static_assert(initList.size() == height_ && initList[0].size() == width_);
  for (std::size_t i = 0; i < initList.size(); ++i)
    for (std::size_t j = 0; j < initList[i].size(); ++j)
      operator()(i, j) = initList[i][j];
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
bool Matrix<T, width_, height_>::operator==(const Matrix &other) const {
  return std::equal(data_, data_ + width_ * height_, other.data_);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
bool Matrix<T, width_, height_>::operator!=(const Matrix &other) const {
  return !operator==(other);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> Matrix<T, width_, height_>::operator-() const {
  return *this * T(-1);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>
Matrix<T, width_, height_>::operator+(const Matrix &other) const {
  Matrix result(1);
  std::transform(data_, data_ + width_ * height_, other.data_, result.data_,
                 std::plus());
  return result;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>
Matrix<T, width_, height_>::operator-(const Matrix &other) const {
  return *this + -other;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>
Matrix<T, width_, height_>::operator*(T scalar) const {
  Matrix result(1);
  std::transform(data_, data_ + width_ * height_, result.data_,
                 [scalar](T element) { return scalar * element; });
  return result;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_>
Matrix<T, width_, height_>::operator/(T scalar) const {
  return *this * (T(1) / scalar);
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t otherWidth>
Matrix<T, otherWidth, height_> Matrix<T, width_, height_>::operator*(
    const Matrix<T, otherWidth, width_> &other) const {
  Matrix<T, otherWidth, height_> result;
  for (std::size_t i = 0; i < height_; ++i)
    for (std::size_t j = 0; j < otherWidth; ++j)
      for (std::size_t k = 0; k < width_; ++k)
        result(i, j) += operator()(i, k) * other(k, j);
  return result;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
typename std::enable_if_t<w == 1 || h == 1, T>
Matrix<T, width_, height_>::dot(const Matrix &other) const {
  // if constexpr (w == 1) {
  //   const Matrix<T, height_, width_> &t = transposed();
  //   return std::inner_product(t.begin(), t.begin() + h,
  //                             other.transposed().begin(), T());
  // } else {
  //   return std::inner_product(data_, data_ + w, other.data_, T());
  // }
  return std::inner_product(data_, data_ + w * h, other.data_, T());
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
typename std::enable_if_t<(w == 1 && h == 3) || (h == 1 && w == 3),
                          Matrix<T, w, h>>
Matrix<T, width_, height_>::cross(const Matrix &other) const {
  // TODO: cross product of two vectors
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
typename std::enable_if_t<w == 1 || h == 1, T>
Matrix<T, width_, height_>::length() const {
  return std::sqrt(dot(*this));
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator+=(const Matrix &other) const {
  return *this = *this + other;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator-=(const Matrix &other) const {
  return *this = *this - other;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator*=(T scalar) const {
  return *this = *this * scalar;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, width_, height_> &
Matrix<T, width_, height_>::operator/=(T scalar) const {
  return *this = *this / scalar;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
T &Matrix<T, width_, height_>::operator()(std::size_t row, std::size_t col) {
  return data_[row * width_ + col];
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
T Matrix<T, width_, height_>::operator()(std::size_t row,
                                         std::size_t col) const {
  return data_[row * width_ + col];
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
std::enable_if_t<w == 1 || h == 1, T>
Matrix<T, width_, height_>::operator()(std::size_t index) const {
  if constexpr (w == 1) {
    return data_[width_ * index];
  } else {
    return data_[index];
  }
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
Matrix<T, height_, width_> Matrix<T, width_, height_>::transposed() const {
  Matrix<T, height_, width_> result(1);
  for (std::size_t i = 0; i < height_; ++i)
    for (std::size_t j = 0; j < width_; ++j)
      result(j, i) = operator()(i, j);
  return result;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
typename std::enable_if_t<width_ == height_, void>
Matrix<T, width_, height_>::transpose() {
  *this = std::move(transposed());
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
std::size_t Matrix<T, width_, height_>::width() const {
  return width_;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
std::size_t Matrix<T, width_, height_>::height() const {
  return height_;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
template <std::size_t w, std::size_t h>
typename std::enable_if_t<w == h, std::size_t>
Matrix<T, width_, height_>::order() const {
  return width_;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
const T *Matrix<T, width_, height_>::begin() const {
  return data_;
}

template <std::floating_point T, std::size_t width_, std::size_t height_>
const T *Matrix<T, width_, height_>::end() const {
  return data_ + width_ * height_;
}

// non-member stuff

template <std::floating_point T, std::size_t width, std::size_t height>
Matrix<T, width, height> operator*(T scalar,
                                   const Matrix<T, width, height> &m) {
  return m * scalar;
}

template <std::floating_point T, std::size_t width, std::size_t height>
Matrix<T, width, height> operator/(T scalar,
                                   const Matrix<T, width, height> &m) {
  return m / scalar;
}

template <std::floating_point T, std::size_t width, std::size_t height>
std::ostream &operator<<(std::ostream &os, const Matrix<T, width, height> &m) {
  for (std::size_t i = 0; i < height; ++i) {
    for (std::size_t j = 0; j < width; ++j)
      os << m(i, j) << " ";
    os << '\n';
  }
  return os << '\b';
}

} // namespace mcpp::math

#endif // MODERN_CPP_INC_MATH_MATRIX_HPP
