#include "ThreadPool.h"
#include<fstream>

ThreadPool::ThreadPool(long noThreads, std::string &stringToFind, std::string &resultFileName, std::string& startDirectory) {
    this->noThreads=noThreads;
    this->threads.reserve(noThreads);
    this->stringToFind=stringToFind;
    this->resultFileName=resultFileName;
    this->startDirectory=startDirectory;

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
    this->directorySearcher = std::thread(&ThreadPool::searchDirectory, this);

    for(int i=0; i<noThreads; i++)
        this->threads.emplace_back(&ThreadPool::startWorkWithFile, this);

    directorySearcher.join();

    for(int i=0; i<noThreads; i++)
        this->threads[i].join();
}

void ThreadPool::searchDirectory() {
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(startDirectory)){
        if(entry.is_regular_file()){
                this->searchedFiles++;
                std::unique_lock lock(mutexQueue);
                addPathToQueue(entry.path());
                lock.unlock();
                emptyQueueCondition.notify_one();
        }
    }
    allFilesFound=true;
}

void ThreadPool::startWorkWithFile(){
    threadIdToPathsMap[std::this_thread::get_id()];
    while(!allFilesFound || !paths.empty()){
        fs::path pathToFile;
        {
            std::unique_lock lock(mutexQueue);
            while(paths.empty()){
                emptyQueueCondition.wait(lock);
            }
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
            mutexFilePathToLineMap.lock();
            this->filePathToLineMap[pathToFile].emplace_back(searchedWordIndex, line);
            mutexFilePathToLineMap.unlock();
        }
    }
    fileToSearch.close();
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

std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>> ThreadPool::getfilePathToLineMap() const {
    return filePathToLineMap;
}