cmake_minimum_required(VERSION 2.8)

project(mempool C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra -pedantic -std=gnu99 -g")
set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

set(HEADER
	./include/mempool.h
	./include/mempool_var.h
)

set(SOURCE
	./src/mempool.c
	./src/mempool_var.c
)

set(TARGET_LIB mempool)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${TARGET_LIB} STATIC ${SOURCE})

install(FILES ${HEADER} DESTINATION ${INSTALL_DIR}/include)
install(TARGETS ${TARGET_LIB} DESTINATION ${INSTALL_DIR}/lib)

add_subdirectory(test)
