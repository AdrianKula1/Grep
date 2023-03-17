#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <string>

#include "Grep.h"

namespace fs=std::filesystem;

Grep::Grep(int argc, char *argv[]) {
    this->argc = argc;
    this->argv = new std::string[argc+1];

    for(int i=0; i<argc; i++)
        this->argv[i] = (std::string) argv[i];


    validateArguments();
    setDefaultArguments();
    setUserArguments();
    this->threadPool = new ThreadPool(numberOfThreads, stringToFind, resultFileName, startDirectory);
}

void Grep::main() {
    this->threadPool->beginWork();
    createResultFile();
    createLogFile();
}

void Grep::validateArguments() const {
    if(argc < this->minNumberOfArgsToRunTheProgram)
        throw std::runtime_error("The program has one obligatory parameter which is <pattern(string)>");
}

void Grep::setDefaultArguments() {
    this->minNumberOfArgsToRunTheProgram = 2;
    this->stringToFind = (std::string)argv[1];
    this->startDirectory = fs::current_path().string();
    this->logFileName = std::filesystem::path(argv[0]).filename().string() + ".log";
    this->resultFileName = std::filesystem::path(argv[0]).filename().string() + ".txt";
    this->numberOfThreads = 4;
}

void Grep::setUserArguments() {
    for(int i=2; i < argc; i+=2){
        if(argv[i] == "-d" || argv[i] == "--dir" )
            this->startDirectory = argv[i + 1];

        if(argv[i] == "-l" || argv[i] == "--log_file" )
            this->logFileName = argv[i + 1] + ".log";

        if(argv[i] == "-r" || argv[i] == "--result_file" )
            this->resultFileName = argv[i + 1] + ".txt";

        if(argv[i] == "-t" || argv[i] == "--threads" ){
            char* p;
            long noThreads = strtol(&argv[i+1][0], &p, 10);
            this->numberOfThreads = noThreads;
        }
    }
}

void Grep::createResultFile() {
    resultDataMapType filePathToLineMap = threadPool->getfilePathToLineMap();

    resultDataVectorType resultDataVector;

    for(auto &data : filePathToLineMap)
        resultDataVector.emplace_back(data);

    std::sort(resultDataVector.begin(), resultDataVector.end(),
              compareData<resultDataVectorItemType>);

    std::ofstream resultFile;
    resultFile.open(resultFileName);

    if(!resultFile)
        return;

    for(auto &threadData: resultDataVector){
        for(auto &pathData: threadData.second)
            resultFile << threadData.first.string() << ":" << pathData.first << ": " << pathData.second << std::endl;
    }

    resultFile.close();
}

void Grep::createLogFile() {
    logDataMapType threadIdToPathsMap = threadPool->getThreadIdToPathsMap();

    logDataVectorType logDataVector;
    for(auto &data : threadIdToPathsMap)
        logDataVector.emplace_back(data);


    std::sort(logDataVector.begin(), logDataVector.end(),
              compareData<logDataVectorItemType>);

    std::ofstream logFile;
    logFile.open(logFileName);

    if(!logFile)
        return;


    for(auto &threadData: logDataVector){
        logFile << threadData.first << ": ";
        for (auto &filename: threadData.second)
            logFile << filename << ',' ;
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

template<typename T>
bool Grep::compareData(T &a, T &b) {
    return a.second.size() > b.second.size();
}








