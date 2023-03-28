#include "tests/int32_type_traits_test.hpp"
#include "type_traits/type_traits.hpp"

#include <iostream>

void testInt32TypeTraits() {
  using type_traits::isInt32;
  std::cout << (isInt32<int>() ? "int is int" : "wtf") << '\n'
            << (isInt32<unsigned>() ? "unsigned is int" : "expected tbh")
            << '\n'
            << (isInt32<float>() ? "honestly this is getting crazy rn"
                                 : "based")
            << std::endl;
}
