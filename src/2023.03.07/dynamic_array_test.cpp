#include "dynamic_array.hpp"
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

int main() {
  testDynamicArray();

  return EXIT_SUCCESS;
}
