cmake_minimum_required(VERSION 3.10)
project(words_counter)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Og")

add_executable(words_counter words_counter.cpp)
