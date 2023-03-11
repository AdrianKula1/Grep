#include "Grep.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace fs=std::filesystem;

Grep::Grep(int args, char *argv[]) {
    this->argc=args;
    this->argv = new std::string[args+1];
    for(int i=0; i<args; i++){
        this->argv[i]=(std::string)argv[i];
    }

}

void Grep::main() {
    validateArguments();

    if(argc == this->minNumberOfArgsToRunTheProgram)
        setDefaultArguments();

    setUserArguments();

}

void Grep::validateArguments() {
    if(argc < this->minNumberOfArgsToRunTheProgram){
        std::cout << "The program has one obligatory parameter which is <pattern(string)>" << std::endl;
        //throw std::runtime_error("error");
    }
}

void Grep::setDefaultArguments() {
    this->STRING_TO_FIND = (std::string)argv[1];
    this->START_DIRECTORY = fs::current_path().string();
    this->LOG_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".log";
    this->RESULT_FILE_NAME = std::filesystem::path(argv[0]).filename().string()+".txt";
    this->NUMBER_OF_THREADS = 4;

}

void Grep::setUserArguments() {
    for(int i=2; i < argc; i+=2){
        if(argv[i]=="-d" || argv[i]=="--dir" ){
            this->START_DIRECTORY =  argv[i+1];
        }
        if(argv[i]== "-l" || argv[i]=="--log_file" ){
            this->LOG_FILE_NAME = argv[i+1]+".log";
        }
        if(argv[i]=="-r" || argv[i]=="--result_file" ){
            this->RESULT_FILE_NAME = argv[i+1]+".txt";
        }
        if(argv[i]=="-t" || argv[i]=="--threads" ){
            char* p;
            long noThreads = strtol(&argv[i+1][0], &p, 10);
            this->NUMBER_OF_THREADS = noThreads;
        }
    }

}


