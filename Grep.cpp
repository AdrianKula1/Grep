#include <string>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Grep.h"
namespace fs=std::filesystem;

Grep::Grep(int args, char *argv[]) {
    this->argc=args;
    this->argv = new std::string[args+1];
    for(int i=0; i<args; i++) {
        this->argv[i] = (std::string) argv[i];
    }
    validateArguments();

    if(argc >= this->minNumberOfArgsToRunTheProgram)
        setDefaultArguments();

    setUserArguments();

    this->threadPool=new ThreadPool(NUMBER_OF_THREADS, STRING_TO_FIND, RESULT_FILE_NAME);
}

void Grep::main() {
    searchDirectory();
    this->threadPool->beginWork();
    createLogFile();
}



void Grep::validateArguments() {
    if(argc < this->minNumberOfArgsToRunTheProgram){
        std::cout << "The program has one obligatory parameter which is <pattern(string)>" << std::endl;
        throw std::runtime_error("error");
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

void Grep::searchDirectory() {
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(START_DIRECTORY)){
        if(entry.is_regular_file()){
            threadPool->addPathToQueue(entry.path());
        }
    }
}

void Grep::createLogFile() {
    std::map<std::thread::id, std::vector<fs::path>> logData = threadPool->getLogData();

    std::vector<std::pair<std::thread::id, std::vector<fs::path>>> logDataVector;
    for(auto &data : logData){
        logDataVector.emplace_back(data);
    }

    std::sort(logDataVector.begin(), logDataVector.end(), compareLogData);

    std::ofstream logFile(LOG_FILE_NAME);

    if(!logFile.good() || !logFile.is_open()){
        return;
    }

    for(auto &threadData: logDataVector){
        logFile << threadData.first << ": ";
        for (auto &filename: threadData.second){
            logFile << filename << ',' ;
        }
        logFile << std::endl;
    }

    logFile.close();
}

unsigned int Grep::getSearchedFiles() const {
    return threadPool->getSearchedFiles();;
}

unsigned int Grep::getFilesWithPattern() const {
    return threadPool->getFilesWithPattern();
}

unsigned int Grep::getPatternsNumber() const {
    return threadPool->getPatternsNumber();
}

std::string Grep::getResultFileName() const {
    return RESULT_FILE_NAME;
}

std::string Grep::getLogFileName() const {
    return LOG_FILE_NAME;
}

unsigned int Grep::getNumberOfThreads() const {
    return NUMBER_OF_THREADS;
}






