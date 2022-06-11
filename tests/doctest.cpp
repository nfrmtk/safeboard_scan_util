//
// Created by Lykov on 09.06.2022.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <scan_lib.h>

TEST_CASE("init"){
    scan_util util(std::filesystem::current_path());
    auto ans = util.scan();
    CHECK_EQ(ans.numbers.processed_files, 1);
}