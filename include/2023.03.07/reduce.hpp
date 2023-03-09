#ifndef MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP
#define MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP

auto reduce(auto begin, auto end, auto operation) {
  auto result = *begin++;
  for (; begin != end; ++begin)
    result = operation(result, *begin);
  return result;
}

#endif // MODERN_CPP_INCLUDE_2023_03_07_REDUCE_HPP
