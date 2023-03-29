#ifndef MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP
#define MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP

#include <cstdint>

namespace type_traits {

template <typename T> struct IsInt32 {
  static constexpr const bool value = false;
};

template <> struct IsInt32<std::int32_t> {
  static constexpr const bool value = true;
};

template <typename T> struct IsChar {
  static constexpr const bool value = false;
};

template <> struct IsChar<char> {
  static constexpr const bool value = true;
};

template <> struct IsChar<signed char> {
  static constexpr const bool value = true;
};

template <> struct IsChar<unsigned char> {
  static constexpr const bool value = true;
};

template <> struct IsChar<wchar_t> {
  static constexpr const bool value = true;
};

template <> struct IsChar<char16_t> {
  static constexpr const bool value = true;
};

template <> struct IsChar<char32_t> {
  static constexpr const bool value = true;
};

template <typename T> constexpr auto IsCharV = IsChar<T>::value;

} // namespace type_traits

#endif // MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP
