cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(new_file VERSION 0.1.0 LANGUAGES C CXX)

add_library(new_file new_file.cpp)
