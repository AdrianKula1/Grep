#include "ThreadPool.h"
#include<fstream>
ThreadPool::ThreadPool(long noThreads, std::string &stringToFind, std::string &resultFileName) {
    this->noThreads=noThreads;
    this->threads.reserve(noThreads);
    this->stringToFind=stringToFind;
    this->resultFileName=resultFileName;

    resetResultFile();
}

void ThreadPool::resetResultFile() {
    std::ofstream fileToSearch;
    fileToSearch.open(this->resultFileName);

    if (!fileToSearch.good() || !fileToSearch.is_open())
        return;
    fileToSearch.close();
}

void ThreadPool::beginWork(){
    this->searchedFiles+=this->paths.size();

    for(int i=0; i<noThreads; i++)
        this->threads.emplace_back(&ThreadPool::startWorkWithFile, this);

    for(int i=0; i<noThreads; i++)
        this->threads[i].join();
}

void ThreadPool::startWorkWithFile(){
    threadIdToPathsMap[std::this_thread::get_id()];

    while(!paths.empty()){
        fs::path pathToFile;
        {
            std::lock_guard lock(queueMutex);
            pathToFile = paths.front();
            paths.pop();
        }
        searchWithinFile(pathToFile);
    }
}

void ThreadPool::searchWithinFile(fs::path &pathToFile){
    std::ifstream fileToSearch;
    fileToSearch.open(pathToFile);

    if (!fileToSearch.good() || !fileToSearch.is_open())
        return;

    threadIdToPathsMap[std::this_thread::get_id()].push_back(pathToFile.filename());

    bool isSearchedPatternFound = false;
    std::string line;
    unsigned int searchedWordIndex = 0;

    while (std::getline(fileToSearch, line)) {
        searchedWordIndex = line.find(stringToFind);

        if (searchedWordIndex != std::string::npos) {
            if (!isSearchedPatternFound) {
                isSearchedPatternFound = true;
                filesContainingPattern++;
            }
            patternsNumber++;
            saveLineToResultFile(pathToFile, searchedWordIndex, line);
        }
    }
    fileToSearch.close();
}

void ThreadPool::saveLineToResultFile(fs::path &pathToFile, unsigned int searchedWordIndex, std::string& line){
    std::ofstream resultFile;
    resultFile.open(this->resultFileName, std::ios_base::app);

    if(!resultFile.good() || !resultFile.is_open())
        return;

    resultFile << pathToFile.string() << ':' << searchedWordIndex << ": " << line << std::endl;

    resultFile.close();
}

void ThreadPool::addPathToQueue(const fs::path& pathToFile){
    this->paths.push(pathToFile);
}

unsigned int ThreadPool::getSearchedFiles() const{
    return this->searchedFiles;
}

unsigned int ThreadPool::getFilesWithPattern() const{
    return this->filesContainingPattern;
}

unsigned int ThreadPool::getPatternsNumber() const{
    return this->patternsNumber;
}

std::map<std::thread::id, std::vector<fs::path>> ThreadPool::getThreadIdToPathsMap() const{
    return threadIdToPathsMap;
}


