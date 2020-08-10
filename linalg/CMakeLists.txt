cmake_minimum_required(VERSION 3.0)
project(linalg)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
add_executable(matmul matmul.cpp)