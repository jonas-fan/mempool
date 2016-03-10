cmake_minimum_required(VERSION 2.8)

project(mempool-test C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=c99 -g")

add_executable(mempool_test
    ./test.c
)

target_link_libraries(mempool_test
    ${TARGET_LIB}
)
