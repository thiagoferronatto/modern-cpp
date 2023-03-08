cmake_minimum_required(VERSION 3.24)

project(modern_cpp)

include_directories(include include/2023.03.07)

add_compile_options(-W -Wall -Wextra -std=c++20 -g)

set(CMAKE_CXX_STANDARD 20)

add_executable(modern_cpp src/2023.03.07/dynamic_array_test.cpp include/2023.03.07/dynamic_array.hpp)
