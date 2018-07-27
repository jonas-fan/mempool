set(HEADER
	${CMAKE_SOURCE_DIR}/lib/mempool.h
	${CMAKE_SOURCE_DIR}/lib/mempool_var.h
)

set(SOURCE
	${CMAKE_SOURCE_DIR}/lib/mempool.c
	${CMAKE_SOURCE_DIR}/lib/mempool_var.c
)

add_library(${TARGET_LIB} STATIC ${SOURCE})

install(FILES ${HEADER} DESTINATION ${INSTALL_DIR}/include)
install(TARGETS ${TARGET_LIB} DESTINATION ${INSTALL_DIR}/lib)