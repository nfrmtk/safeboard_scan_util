//
// Created by Lykov on 08.06.2022.
//

#ifndef SCAN_UTIL_SCAN_LIB_H
#define SCAN_UTIL_SCAN_LIB_H
#include <filesystem>
#include <chrono>
#include <ctime>
#include <future>
#include <thread>
#include <algorithm>
#include <set>
#include <fstream>
#include <cassert>
#include <regex>
namespace fs = std::filesystem;

class scan_util{
    using danger_vector_t = std::vector<fs::path>;
    struct dangers_chunk{
        danger_vector_t dangers;
        size_t total_size;
        void add(const fs::path& danger ){
            dangers.push_back(danger);
            total_size += fs::file_size(danger);
        }
    };
    struct less_for_chunks_t{
        bool operator() (const dangers_chunk& first, const dangers_chunk& second) const { return first.total_size > second.total_size; }
    };
    using danger_chunks_tree_t = std::multiset<dangers_chunk, less_for_chunks_t>;
    fs::path working_directory;


    void add_danger_to_set(danger_chunks_tree_t& chunks_set, const fs::path& danger);
    static bool is_significantly_bigger(uintmax_t big, uintmax_t small );

    danger_vector_t paths_to_all_possible_dangers();
    static bool is_possible_danger(const fs::path& path);
    danger_chunks_tree_t split_dangers_into_chunks(uint32_t chinks, const danger_vector_t& dangers);



public:
    struct info_counts{
        size_t processed_files, js_detects, cmd_detects, exe_detects, errors;
        info_counts& operator+= (const info_counts& other);
    };

    struct info{
        info_counts numbers;
        std::time_t time_for_scanning;
    };

    explicit  scan_util(fs::path && directory) : working_directory(std::move(directory)) {}
    info scan();


private:
    static info_counts scan_chunk(const dangers_chunk& dangers);
    static info_counts scan_file(const fs::path& file);
    static bool check_if_string_is_bad(const std::string& line, const fs::path& extension);
};



#endif //SCAN_UTIL_SCAN_LIB_H
