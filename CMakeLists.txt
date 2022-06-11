cmake_minimum_required(VERSION 3.22)
project(scan_util)

set(CMAKE_CXX_STANDARD 20)
include_directories(core_lib tests thirdparty io_lib)

add_subdirectory(core_lib)
add_subdirectory(tests)

add_executable(scan_util main.cpp io_lib/io_helper.h)

target_link_libraries(scan_util scan_lib)
