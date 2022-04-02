cmake_minimum_required(VERSION 3.5)
project(pnk-rb-tree VERSION 1.1)

set(library-source
	inc/pnk/rb_tree.h
	src/rb_tree.c
)

add_library(pnk-rb-tree INTERFACE)
target_include_directories(pnk-rb-tree INTERFACE inc)
target_compile_features(pnk-rb-tree INTERFACE c_std_99)

add_executable(insert_test tests/insert_test.c ${library-source} tests/utility.c)
add_executable(delete_test tests/delete_test.c ${library-source} tests/utility.c)
add_executable(minmax_test tests/minmax_test.c ${library-source} tests/utility.c)
target_link_libraries(insert_test PRIVATE pnk-rb-tree)
target_link_libraries(delete_test PRIVATE pnk-rb-tree)
target_link_libraries(minmax_test PRIVATE pnk-rb-tree)
