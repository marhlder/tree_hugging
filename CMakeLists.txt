cmake_minimum_required(VERSION 3.23)
project(tree_hugging)

set(CMAKE_CXX_STANDARD 23)

add_executable(tree_hugging main.cpp btree.cpp btree.h)
