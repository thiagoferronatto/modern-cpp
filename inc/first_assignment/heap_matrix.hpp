#ifndef MODERN_CPP_INC_FIRST_ASSIGNMENT_HEAP_MATRIX_HPP
#define MODERN_CPP_INC_FIRST_ASSIGNMENT_HEAP_MATRIX_HPP

#include "math/matrix.hpp"
#include <memory>

namespace mcpp::math::p1 {

template <std::floating_point T> class HeapMatrix {
public:
  HeapMatrix(std::size_t width, std::size_t height)
      : matrix_(std::make_shared<DMatrix<T>>(width, height)) {}

  HeapMatrix(const HeapMatrix &other) : matrix_(other.matrix_) {}

  HeapMatrix(HeapMatrix &&other) noexcept : matrix_(std::move(other.matrix_)) {}

  HeapMatrix &operator=(const HeapMatrix &other) {
    if (this == &other)
      goto skipCopy;
    matrix_ = other.matrix_;
  skipCopy:
    return *this;
  }

  HeapMatrix &operator=(HeapMatrix &&other) noexcept {
    if (this == &other)
      goto skipMove;
    matrix_ = std::move(other.matrix_);
  skipMove:
    return *this;
  }

  T operator()(std::size_t row, std::size_t column) const {
    return matrix_->operator()(row, column);
  }

  T &operator()(std::size_t row, std::size_t column) {
    return matrix_->operator()(row, column);
  }

  static HeapMatrix clone(const HeapMatrix &source) {
    HeapMatrix result;
    result.matrix_ = std::make_shared<DMatrix<T>>(source.matrix_->width(),
                                                  source.matrix_->height());
    std::copy(source.matrix_->begin(), source.matrix_->end(),
              result.matrix_->begin());
    return result;
  }

  HeapMatrix operator+(const HeapMatrix &other) const {
    return HeapMatrix(matrix_->operator+(*other.matrix_.get()));
  }

  // TODO: finish implementing relevant methods

private:
  HeapMatrix() = default;

  explicit HeapMatrix(const DMatrix<T> &dMatrix)
      : matrix_(std::make_shared<DMatrix<T>>(dMatrix)) {}

  std::shared_ptr<DMatrix<T>> matrix_ = nullptr;
};

// aliases

template <std::floating_point T> using HMatrix = HeapMatrix<T>;

using HFMatrix = HMatrix<float>;

} // namespace mcpp::math::p1

#endif // MODERN_CPP_INC_FIRST_ASSIGNMENT_HEAP_MATRIX_HPP
