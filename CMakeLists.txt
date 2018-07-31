cmake_minimum_required(VERSION 2.8)

project(libmempool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -g -std=c99")

add_subdirectory(tests)