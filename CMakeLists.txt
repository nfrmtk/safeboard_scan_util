cmake_minimum_required(VERSION 3.22)
project(scan_util)

set(CMAKE_CXX_STANDARD 20)
include_directories(lib)
add_subdirectory(lib)

add_executable(scan_util main.cpp)
target_link_libraries(scan_util scan_lib)
