#ifndef MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP
#define MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP

namespace class_of_2023_03_07 {

auto reduce(auto begin, auto end, auto operation, auto initialValue) {
  auto result = initialValue;
  for (; begin != end; ++begin)
    result = operation(result, *begin);
  return result;
}

} // namespace class_of_2023_03_07

#endif // MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP
