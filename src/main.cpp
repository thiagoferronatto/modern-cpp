#include "tests/dynamic_array_and_reduction_tests.hpp"
#include "tests/fundamental_types_tests.hpp"
#include "tests/int32_type_traits_test.hpp"
#include "tests/linked_list_test.hpp"
#include <cstdlib>

int main() {
  testDynamicArray();
  testReduction();
  testLinkedList();
  testInt32TypeTraits();
  testFundamentalTypes();

  return EXIT_SUCCESS;
}
