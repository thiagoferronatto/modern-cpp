#ifndef MODERN_CPP_INC_MATH_MATRIX_HPP
#define MODERN_CPP_INC_MATH_MATRIX_HPP

#include "math/static_matrix.hpp"

namespace mcpp::math {

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
  Matrix &operator+=(const Matrix<T, w, h> &other) {
    return *this = *this + other;
  }

  template <std::size_t w, std::size_t h>
  Matrix &operator-=(const Matrix<T, w, h> &other) {
    return *this = *this - other;
  }

  Matrix &operator*=(T scalar) { return *this = *this * scalar; }

  Matrix &operator/=(T scalar) { return *this = *this / scalar; }

  template <std::size_t w, std::size_t h>
  Matrix &operator*=(const Matrix<T, w, h> &other) {
    return *this = *this * other;
  }

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

  [[maybe_unused]] void transpose() { *this = transposed(); }

  [[nodiscard]] std::size_t width() const { return width_; }

  [[nodiscard]] std::size_t height() const { return height_; }

private:
  std::size_t width_, height_;
  T *data_;
};

// aliases

template <std::floating_point T> using DMatrix = Matrix<T, 0, 0>;

using DFMatrix [[maybe_unused]] = DMatrix<float>;

} // namespace mcpp::math

#endif // MODERN_CPP_INC_MATH_MATRIX_HPP
