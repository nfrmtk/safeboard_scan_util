//
// Created by Lykov on 11.06.2022.
//

#ifndef SCAN_UTIL_IO_HELPER_H
#define SCAN_UTIL_IO_HELPER_H
#include <scan_lib.h>
#include <iostream>
namespace io_helper{
    std::string make_working_directory(const std::vector<std::string>& directory_vector){
        std::string ans = "";
        for (const auto& str: directory_vector) {
            ans+= (str + " ");
        }
        return ans.substr(0, ans.size() - 1);
    }

    void output_time(const std::time_t& time){
        uint16_t ms = time%1000, seconds = (time/1000)%60, minutes = time/60000;
        std::cout << std::setw(20) << "Executaion time: ";
        if (ms) std::cout << ms << "ms";
        if (seconds) std::cout <<  ", " <<seconds << "s ";
        if (minutes) std::cout << ", " <<minutes << "min\n";
    }

    void output_result(const scan_util::info& results){
        std::cout << "\n\nscan_util have finished working. Here are the results:\n";
        std::cout << std::setw(20) <<"Processed files:   " << results.numbers.processed_files << '\n';
        std::cout << std::setw(20) <<"JS detects:   " << results.numbers.js_detects << '\n';
        std::cout << std::setw(20) <<"CMD detects:   " << results.numbers.cmd_detects << '\n';
        std::cout << std::setw(20) <<"EXE detects:   " << results.numbers.exe_detects << '\n';
        std::cout << std::setw(20) <<"Errors:   " << results.numbers.errors << '\n';
        output_time(results.time_for_scanning);
    }

}; // io_helper
#endif //SCAN_UTIL_IO_HELPER_H
