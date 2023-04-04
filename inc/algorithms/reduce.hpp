#ifndef MODERN_CPP_INC_ALGORITHMS_REDUCE_HPP
#define MODERN_CPP_INC_ALGORITHMS_REDUCE_HPP

namespace mcpp::algorithms {

auto reduce(auto begin, auto end, auto operation, auto initialValue) {
  auto result = initialValue;
  for (; begin != end; ++begin)
    result = operation(result, *begin);
  return result;
}

} // namespace data_structures

#endif // MODERN_CPP_INC_ALGORITHMS_REDUCE_HPP
