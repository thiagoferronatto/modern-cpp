#include "tests/dynamic_array_and_reduction_tests.hpp"
#include "tests/linked_list_test.hpp"
#include <cstdlib>

int main() {
  testDynamicArray();
  testReduction();
  testLinkedList();

  return EXIT_SUCCESS;
}