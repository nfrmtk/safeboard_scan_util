//
// Created by Lykov on 09.06.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <scan_lib.h>

TEST_CASE("init"){
    scan_util util("C:\\Users\\Lykov\\Documents\\code_playground");
    auto ans = util.scan();
    CHECK_GE(ans.numbers.processed_files, 1);
}