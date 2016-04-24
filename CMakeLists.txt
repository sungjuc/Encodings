cmake_minimum_required(VERSION 3.2)
project(encodings)

include_directories(${CMAKE_BINARY_DIR})
add_subdirectory(src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")