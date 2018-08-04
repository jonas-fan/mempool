cmake_minimum_required(VERSION 2.8)

project(libmempool-test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -pedantic -std=c99 -g")

add_executable(unit_test ./unit_test.c)
target_link_libraries(unit_test ${TARGET_LIB})

add_executable(stress_test ./stress_test.c)
target_link_libraries(stress_test ${TARGET_LIB})