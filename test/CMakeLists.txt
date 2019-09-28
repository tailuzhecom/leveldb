cmake_minimum_required(VERSION 3.9)
# Keep the version below in sync with the one in db.h
project(filter_block_test1)

# This project requires C++11.
set(CMAKE_CXX_STANDARD 11)

add_executable(filter_block_test filter_block_test.cpp)