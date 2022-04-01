cmake_minimum_required(VERSION 3.5)
project(pnk-rb-tree VERSION 1.1)

set(library-source
	inc/pnk/rb_tree.h
	src/rb_tree.c
)

add_library(pnk-rb-tree INTERFACE)
target_include_directories(pnk-rb-tree INTERFACE inc)
target_compile_features(pnk-rb-tree INTERFACE c_std_99)
