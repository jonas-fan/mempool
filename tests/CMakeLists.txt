cmake_minimum_required(VERSION 2.8)

project(mempool-test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -pedantic -std=c99 -g")

add_executable(mempool_test ./mempool_test.c)
add_executable(mempool_stress_test ./mempool_stress_test.c)
