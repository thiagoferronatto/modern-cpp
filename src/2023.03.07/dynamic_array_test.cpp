#include "dynamic_array.hpp"
#include "reduce.hpp"
#include <cstdlib>
#include <iostream>

auto testDynamicArray() {
  using class_of_2023_03_07::Array;

  Array<unsigned> a;

  for (auto i = 0U; i < 11; ++i)
    a.push(i);

  std::cout << "a = " << a << std::endl;

  if (a.contains(3))
    std::cout << "a contains 3" << std::endl;

  a.remove(3);

  std::cout << "a = " << a << std::endl;

  a.clear();

  std::cout << "a = " << a << std::endl;

  Array<float> b{1, 2, 3, 4, 5};

  std::cout << "b = " << b << std::endl;
}

auto testReduction() {
  using class_of_2023_03_07::Array;

  Array<float> array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  std::cout << "reduction: "
            << reduce(std::cbegin(array), std::cend(array), std::plus())
            << std::endl;
}

int main() {
  testDynamicArray();

  testReduction();

  return EXIT_SUCCESS;
}
