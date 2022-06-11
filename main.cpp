#include <iostream>


#include <argparse.hpp>

#include <io_helper.h>


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
        std::string working_directory = io_helper::make_working_directory(working_directory_vector);
        scan_util scanUtil(working_directory);
        auto scan_info = scanUtil.scan();
        io_helper::output_result(scan_info);
    }
    catch(const std::exception& err){
        std::cerr << err.what() << "occured. The directory is incorrect\n";
        std::cerr << ap;
    }

    return 0;
}
