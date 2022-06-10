//
// Created by Lykov on 08.06.2022.
//

#include "scan_lib.h"

scan_util::info scan_util::scan() {
    info answer{};
    auto init_time = std::chrono::system_clock::now();

    uint32_t THREADS = 2 * std::thread::hardware_concurrency();
    danger_vector_t dangers = paths_to_all_possible_dangers();
    std::sort(dangers.begin(), dangers.end(), [] (const fs::path& first, const fs::path& second )
        {
            return fs::file_size(first) > fs::file_size(second);
        });
    auto chunks = split_dangers_into_chunks(THREADS, dangers);
    std::vector<std::future<info_counts>> future_answers;
    for (auto it = chunks.begin(); it != chunks.end(); ++it){
        future_answers.push_back(
                std::async(std::launch::async, scan_chunk, *it )
                );
    }
    for (auto& future_answer: future_answers){
        answer.numbers += future_answer.get();
    }
    answer.time_for_scanning = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - init_time).count();
    return answer;
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

scan_util::danger_chunks_tree_t scan_util::split_dangers_into_chunks(uint32_t chunks, const danger_vector_t & dangers) {
    std::multiset<dangers_chunk, less_for_chunks_t> chunks_set;
    for (uint32_t i = 0; i < chunks; ++i) chunks_set.emplace(); // filling set with chunks number of empty values
    for (const fs::path& danger: dangers){
        add_danger_to_set(chunks_set, danger);
    }
    return chunks_set;
}

bool scan_util::is_significantly_bigger(uintmax_t big, uintmax_t small) {
    return big / 20 > small;
}

void scan_util::add_danger_to_set(scan_util::danger_chunks_tree_t &chunks_set, const fs::path& danger) {
    if (is_significantly_bigger(chunks_set.begin()->total_size, fs::file_size(danger)) ){ // if so, there will be little change in the tree
        auto hint = chunks_set.begin();
        auto data = chunks_set.extract(hint);
        data.value().add(danger);
        chunks_set.emplace_hint(chunks_set.begin(), std::move(data.value()));
    }
    else{
        auto data = chunks_set.extract(chunks_set.begin()).value();
        data.add(danger);
        chunks_set.emplace(std::move(data));
    }
}

scan_util::info_counts scan_util::scan_chunk(const scan_util::dangers_chunk& chunk) {
    info_counts answer{};
    for (const fs::path& danger : chunk.dangers ){
        answer+= scan_file(danger);
    }
    return answer;
}

scan_util::info_counts scan_util::scan_file(const fs::path &danger) {
    std::ifstream danger_stream(danger);

    info_counts answer{};
    if (!danger_stream.is_open()) {
        ++answer.errors;
        return answer;
    }
    std::regex bad_substr;
    bool is_target_extension = false;
    size_t info_counts::*  file_type_counter;
    if (danger.extension() == ".cmd" || danger.extension() == ".bat" ) {
        file_type_counter = &info_counts::cmd_detects;
        is_target_extension = true;
    }
    if (danger.extension() == ".js") {
        is_target_extension = true;
        file_type_counter = &info_counts::js_detects;
    }
    if (danger.extension() == ".exe" || danger.extension() == ".dll" ) {
        is_target_extension = true;
        file_type_counter = &info_counts::exe_detects;
    }
    if (is_target_extension) {
        std::string temp;
        ++answer.processed_files;
        while (std::getline(danger_stream, temp)) {
            if (check_if_string_is_bad(temp, danger.extension())) {
                ++(answer.*file_type_counter);
                break;
            }
        }
    }
    return answer;
}

bool scan_util::check_if_string_is_bad(const std::string& line, const fs::path& extension ) {
    if (extension == ".js"){
        return line.find("<script>evil_script()</script>") != -1;
    }
    if (extension == ".cmd" || extension == ".bat"){
        return line.find("rd /s /q \"c:\\windows\"") != -1;
    }
    if (extension == ".dll" || extension == ".exe"){
        return line.find("CreateRemoteThread") != -1 || line.find("CreateProcess") != -1;
    }
    return false;
}


scan_util::info_counts &scan_util::info_counts::operator+=(const scan_util::info_counts &other) {
    processed_files += other.processed_files;
    js_detects += other.js_detects;
    cmd_detects += other.cmd_detects;
    exe_detects += other.exe_detects;
    errors += other.errors;
    return *this;
}
