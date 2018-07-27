cmake_minimum_required(VERSION 2.8)

project(libmempool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -g -std=c99")

set(TARGET_LIB mempool)
set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install/)

add_subdirectory(lib)
add_subdirectory(tests)

message("The default install directory is '${INSTALL_DIR}'.")