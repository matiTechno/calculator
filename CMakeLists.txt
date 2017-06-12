cmake_minimum_required(VERSION 2.6)
project(calculator)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17 -O0 -g")

add_library(calc calc.cpp calc.hpp)

add_executable(calculator main.cpp)
target_link_libraries(calculator calc)

add_executable(catch_test catch_test.cpp catch.hpp)
target_link_libraries(catch_test calc)

add_executable(google_test google_test.cpp)
target_link_libraries(google_test calc -lgtest -lgtest_main)

enable_testing()
add_test(test1 catch_test)
add_test(test2 google_test)
