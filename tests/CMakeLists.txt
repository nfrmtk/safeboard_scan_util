add_executable(doctest doctest.cpp ../io_lib/io_helper.h)

target_link_libraries(doctest scan_lib)
