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

} // namespace type_traits

#endif // MODERN_CPP_INC_TYPE_TRAITS_TYPE_TRAITS_HPP
