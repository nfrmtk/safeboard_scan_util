#include <iostream>


#include <scan_lib.h>
#include <argparse.hpp>

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


int main(int argc, char* argv[]) {

    argparse::ArgumentParser ap("scan_util", "1.0");
    ap.add_argument("directory").remaining();

    try{
        ap.parse_args(argc, argv);
    }
    catch(const std::exception& err){
        std::cerr << err.what() <<  " occured. could not parse path.";
        std::cerr << ap;
    }

    try{
        auto working_directory_vector = ap.get<std::vector<std::string>>("directory");
        std::string working_directory = make_working_directory(working_directory_vector);
        scan_util scanUtil(working_directory);
        auto scan_info = scanUtil.scan();
        output_result(scan_info);
    }
    catch(const std::exception& err){
        std::cerr << err.what() << "occured. The directory is incorrect\n";
        std::cerr << ap;
    }

    return 0;
}
