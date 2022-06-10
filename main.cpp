#include <iostream>


#include <scan_lib.h>
#include <argparse.hpp>


void output_result(const scan_util::info& results){
    std::cout << "\n\nscan_util have finished working. Here are the results:\n";
    std::cout << std::setw(20) <<"Processed files:   " << results.numbers.processed_files << '\n';
    std::cout << std::setw(20) <<"JS detects:   " << results.numbers.js_detects << '\n';
    std::cout << std::setw(20) <<"CMD detects:   " << results.numbers.cmd_detects << '\n';
    std::cout << std::setw(20) <<"EXE detects:   " << results.numbers.exe_detects << '\n';
    std::cout << std::setw(20) <<"Errors:   " << results.numbers.errors << '\n';
    std::cout << std::setw(20) << "Executaion time:   " << results.time_for_scanning << "ms\n";
}

int main(int argc, char* argv[]) {

    argparse::ArgumentParser ap("scan_util", "1.0");
    ap.add_argument("directory");

    try{
        ap.parse_args(argc, argv);
    }
    catch(const std::exception& err){
        std::cerr << err.what() <<  " occured. could not parse path.";
        std::cerr << ap;
    }

    try{
        auto working_directory = ap.get<std::string>("directory");
        scan_util scanUtil(working_directory);
        auto scan_info = scanUtil.scan();
        output_result(scan_info);
    }
    catch(const std::exception& err){
        std::cout << err.what() << "occured. The directory is incorrect\n";
        std::cerr << ap;
    }
    return 0;
}
