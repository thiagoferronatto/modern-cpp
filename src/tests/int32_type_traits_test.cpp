#include "tests/int32_type_traits_test.hpp"
#include "type_traits/type_traits.hpp"

#include <iostream>

void testInt32TypeTraits() {
  using type_traits::IsInt32;
  std::cout << (IsInt32<int>::value ? "int is int" : "wtf") << '\n'
            << (IsInt32<unsigned>::value ? "unsigned is int" : "expected tbh")
            << '\n'
            << (IsInt32<float>::value ? "honestly this is getting crazy rn"
                                      : "based")
            << std::endl;
}
