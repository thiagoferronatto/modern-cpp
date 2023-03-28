#include "tests/dynamic_array_and_reduction_tests.hpp"
#include "algorithms/reduce.hpp"
#include "data_structures/dynamic_array.hpp"
#include <iostream>

void testDynamicArray() {
  using data_structures::Array;

  Array<unsigned> a;

  for (auto i = 0U; i < 11; ++i)
    a.push(i);

  std::cout << "a = " << a << std::endl;

  if (a.contains(3))
    std::cout << "a contains 3" << std::endl;

  std::cout << "removed 3? " << a.remove(3) << std::endl;

  std::cout << "removed 20? " << a.remove(20) << std::endl;

  std::cout << "a = " << a << std::endl;

  a.clear();

  std::cout << "a = " << a << std::endl;

  Array<float> b{1, 2, 3, 4, 5};

  std::cout << "b = " << b << std::endl;
}

void testReduction() {
  using algorithms::reduce;
  using data_structures::Array;

  Array<float> array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::cout << "reduction: "
            << reduce(std::cbegin(array), std::cend(array), std::plus(), 0.0F)
            << std::endl;
}
