#include "dynamic_array_and_reduction_tests.hpp"
#include "linked_list_test.hpp"
#include <cstdlib>

int main() {
  testDynamicArray();
  testReduction();
  testLinkedList();

  return EXIT_SUCCESS;
}