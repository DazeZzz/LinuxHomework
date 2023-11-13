cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ls VERSION 0.1.0 LANGUAGES C CXX)

add_executable(ls file_status.cpp main.cpp)