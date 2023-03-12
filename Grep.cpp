#include <string>
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Grep.h"
namespace fs=std::filesystem;

Grep::Grep(int argc, char *argv[]) {
    this->argc=argc;
    this->argv = new std::string[argc+1];
    for(int i=0; i<argc; i++) {
        this->argv[i] = (std::string) argv[i];
    }

    validateArguments();
    setDefaultArguments();
    setUserArguments();
    this->threadPool = new ThreadPool(numberOfThreads, stringToFind, resultFileName, startDirectory);
}

void Grep::main() {
    this->threadPool->beginWork();
    createLogFile();
}

void Grep::validateArguments() const {
    if(argc < this->minNumberOfArgsToRunTheProgram){
        std::cout << "The program has one obligatory parameter which is <pattern(string)>" << std::endl;
        throw std::runtime_error("error");
    }
}

void Grep::setDefaultArguments() {
    this->stringToFind = (std::string)argv[1];
    this->startDirectory = fs::current_path().string();
    this->logFileName = std::filesystem::path(argv[0]).filename().string() + ".log";
    this->resultFileName = std::filesystem::path(argv[0]).filename().string() + ".txt";
    this->numberOfThreads = 4;

}

void Grep::setUserArguments() {
    for(int i=2; i < argc; i+=2){
        if(argv[i]=="-d" || argv[i]=="--dir" ){
            this->startDirectory =  argv[i + 1];
        }
        if(argv[i]== "-l" || argv[i]=="--log_file" ){
            this->logFileName = argv[i + 1] + ".log";
        }
        if(argv[i]=="-r" || argv[i]=="--result_file" ){
            this->resultFileName = argv[i + 1] + ".txt";
        }
        if(argv[i]=="-t" || argv[i]=="--threads" ){
            char* p;
            long noThreads = strtol(&argv[i+1][0], &p, 10);
            this->numberOfThreads = noThreads;
        }
    }
}

void Grep::createLogFile() {
    std::map<std::thread::id, std::vector<fs::path>> threadIdToPathsMap = threadPool->getThreadIdToPathsMap();

    std::vector<std::pair<std::thread::id, std::vector<fs::path>>> logDataVector;
    for(auto &data : threadIdToPathsMap){
        logDataVector.emplace_back(data);
    }

    std::sort(logDataVector.begin(), logDataVector.end(), compareLogData);

    std::ofstream logFile(logFileName);

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
    return threadPool->getSearchedFiles();
}

unsigned int Grep::getFilesWithPattern() const {
    return threadPool->getFilesWithPattern();
}

unsigned int Grep::getPatternsNumber() const {
    return threadPool->getPatternsNumber();
}

std::string Grep::getResultFileName() const {
    return resultFileName;
}

std::string Grep::getLogFileName() const {
    return logFileName;
}

unsigned int Grep::getNumberOfThreads() const {
    return numberOfThreads;
}

bool Grep::compareLogData(std::pair<std::thread::id, std::vector<fs::path>> &a,
                          std::pair<std::thread::id, std::vector<fs::path>> &b) {
    return a.second.size() > b.second.size();
}






