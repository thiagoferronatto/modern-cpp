#ifndef MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP
#define MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP

#include <cstdint>

namespace type_traits {

template <typename T> inline constexpr bool isInt32() { return false; }

template <> inline constexpr bool isInt32<std::int32_t>() { return true; }

} // namespace type_traits

#endif // MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP
