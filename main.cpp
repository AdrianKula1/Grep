#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
namespace fs=std::filesystem;

int main(int args, char *argv[]){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if(args<2){
        std::cout << "The program has one obligatory parameter which is <pattern(string)>" << std::endl;
        return 0;
    }
    //default parameters
    std::string START_DIRECTORY = fs::current_path().string();
    std::string LOG_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".log";
    std::string RESULT_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".txt";
    long NUMBER_OF_THREADS = 4;

    if(args>2){
        for(int i=2; i<args; i+=2){
            if((std::string)argv[i]=="-d" || (std::string)argv[i]=="--dir" ){
                START_DIRECTORY = (std::string) argv[i+1];
            }
            if((std::string)argv[i]== "-l" || (std::string)argv[i]=="--log_file" ){
                LOG_FILE_NAME = (std::string) argv[i+1]+".log";
            }
            if((std::string)(argv[i])=="-r" || (std::string)argv[i]=="--result_file" ){
                RESULT_FILE_NAME = (std::string) argv[i+1]+".txt";
            }
            if((std::string)(argv[i])=="-t" || (std::string)argv[i]=="--threads" ){
                char* p;
                long arg = strtol(argv[i+1], &p, 10);
                NUMBER_OF_THREADS = arg;
            }
        }
    }
    std::cout << START_DIRECTORY << std::endl;
    std::cout << LOG_FILE_NAME << std::endl;
    std::cout << RESULT_FILE_NAME << std::endl;
    std::cout << NUMBER_OF_THREADS << std::endl;

    int searchedFiles=0, filesWithPattern=0, patternsNumber=0;
    std::string resultFilePath, logFilePath;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long int elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cout << elapsed_time << std::endl;
    std::cout << typeid(elapsed_time).name() << std::endl;

    return 0;
}
