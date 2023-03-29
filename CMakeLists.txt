cmake_minimum_required(VERSION 3.18)

project(modern_cpp)

include_directories(inc inc/2023.03.07 inc/2023.03.09)

add_compile_options(-W -Wall -Wextra -std=c++23 -g)

set(CMAKE_CXX_STANDARD 23)

add_executable(modern_cpp src/tests/dynamic_array_and_reduction_tests.cpp
        inc/data_structures/dynamic_array.hpp
        inc/data_structures/linked_list.hpp
        src/tests/linked_list_test.cpp src/main.cpp
        inc/tests/dynamic_array_and_reduction_tests.hpp
        inc/tests/linked_list_test.hpp
        inc/tests/int32_type_traits_test.hpp
        inc/type_traits/type_traits.hpp
        src/tests/int32_type_traits_test.cpp inc/tests/fundamental_types_tests.hpp src/tests/fundamental_types_tests.cpp)
