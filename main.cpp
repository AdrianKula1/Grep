#include <iostream>
#include <filesystem>

namespace fs=std::filesystem;

int main(int args, char *argv[]){
    if(args<2){
        std::cout << "The program has one obligatory parameter which is <pattern(string)>" << std::endl;
        return 0;
    }
    //default parameters
    std::string START_DIRECTORY = fs::current_path().string();
    std::string LOG_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".log";
    std::string RESULT_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".txt";
    long NUMBER_OF_THREADS = 4;


    std::cout << START_DIRECTORY << std::endl;
    std::cout << LOG_FILE_NAME << std::endl;
    std::cout << RESULT_FILE_NAME << std::endl;
    std::cout << NUMBER_OF_THREADS << std::endl;
    return 0;
}
