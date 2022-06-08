//
// Created by Lykov on 08.06.2022.
//

#include "scan_lib.h"

scan_util::info scan_util::scan() {
    info answer;
    auto current_time = std::chrono::system_clock::now();

    uint32_t THREADS = 2 * std::thread::hardware_concurrency();
    danger_vector_t dangers = paths_to_all_possible_dangers();
    std::sort(dangers.begin(), dangers.end(), [] (const fs::path& first, const fs::path& second )
        {
            return fs::file_size(first) > fs::file_size(second);
        });
    auto chunks = split_dangers_into_chunks(THREADS, dangers);

}

scan_util::danger_vector_t scan_util::paths_to_all_possible_dangers() {
    danger_vector_t answer;
    std::for_each(fs::directory_iterator(working_directory),
                  fs::directory_iterator(),
                  [&answer](const fs::path& path) -> void { if (is_possible_danger(path)) answer.push_back(path);}
                  );
    return answer;
}

bool scan_util::is_possible_danger(const fs::path & path) {
    return path.has_extension() && (path.extension() == ".cmd" ||
                                    path.extension() == ".bat" ||
                                    path.extension() == ".exe" ||
                                    path.extension() == ".dll" ||
                                    path.extension() == ".js" );
}

std::vector<scan_util::dangers_chunk> scan_util::split_dangers_into_chunks(uint32_t chunks, const danger_vector_t & dangers) {
    std::multiset<dangers_chunk, less_for_chunks_t> chunks_set;
    for (uint32_t i = 0; i < chunks; ++i) chunks_set.emplace(); // filling set with chunks number of empty values
    for (const fs::path& danger: dangers){
        add_danger_to_set(chunks_set, danger);
    }
    return std::vector<dangers_chunk>(chunks_set.begin(), chunks_set.end());
}

bool scan_util::is_significantly_bigger(uintmax_t big, uintmax_t small) {
    return big / 20 > small;
}

void scan_util::add_danger_to_set(scan_util::danger_chunks_tree_t &chunks_set, const fs::path& danger) {
    if (is_significantly_bigger(chunks_set.begin()->total_size, fs::file_size(danger)) ){ // if so, there will be little change in the tree
        auto hint = chunks_set.begin();
        auto data = chunks_set.extract(hint);
        data.value().add(danger);
        chunks_set.emplace_hint(hint, std::move(data.value()));
    }
    else{
        auto data = chunks_set.extract(chunks_set.begin()).value();
        data.add(danger);
        chunks_set.emplace(std::move(data));
    }
}



