#include "tests/fundamental_types_tests.hpp"

#include "type_traits/type_traits.hpp"
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

// stolen from https://stackoverflow.com/a/4541470
template <typename T> constexpr std::string demangledName() {
  int status;
  std::unique_ptr<char, void (*)(void *)> res(
      abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
      std::free);

  return status == 0 ? res.get() : typeid(T).name();
}

template <typename T> struct TypeInfo {
  static constexpr auto size = sizeof(T);
  static constexpr auto min = std::numeric_limits<T>::min();
  static constexpr auto max = std::numeric_limits<T>::max();
};

template <typename T> constexpr auto getTypeInfo() {
  std::conditional_t<type_traits::IsCharV<T>, long long, T>
      min = std::numeric_limits<T>::lowest(),
      max = std::numeric_limits<T>::max();
  std::stringstream ss;
  ss << demangledName<T>() << ": " << sizeof(T) << " byte"
     << (sizeof(T) == 1 ? "" : "s") << "; [" << min << ',' << max << ']';
  return ss.str();
}

void testFundamentalTypes() {
  std::cout << getTypeInfo<bool>() << '\n'
            << getTypeInfo<signed char>() << '\n'
            << getTypeInfo<char>() << '\n'
            << getTypeInfo<unsigned char>() << '\n'
            << getTypeInfo<short>() << '\n'
            << getTypeInfo<wchar_t>() << '\n'
            << getTypeInfo<char16_t>() << '\n'
            << getTypeInfo<unsigned short>() << '\n'
            << getTypeInfo<int>() << '\n'
            << getTypeInfo<char32_t>() << '\n'
            << getTypeInfo<unsigned>() << '\n'
            << getTypeInfo<long>() << '\n'
            << getTypeInfo<unsigned long>() << '\n'
            << getTypeInfo<long long>() << '\n'
            << getTypeInfo<unsigned long long>() << '\n'
            << getTypeInfo<float>() << '\n'
            << getTypeInfo<double>() << '\n'
            << getTypeInfo<long double>() << std::endl;
}