cmake_minimum_required(VERSION 3.18)

project(modern_cpp)

include_directories(include include/2023.03.07 include/2023.03.09)

add_compile_options(-W -Wall -Wextra -std=c++20 -g)

set(CMAKE_CXX_STANDARD 20)

add_executable(modern_cpp src/2023.03.07/dynamic_array_and_reduction_tests.cpp include/2023.03.07/dynamic_array.hpp include/2023.03.09/linked_list.hpp src/2023.03.09/linked_list_test.cpp src/main.cpp include/2023.03.07/dynamic_array_and_reduction_tests.hpp include/2023.03.09/linked_list_test.hpp)
