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

    if (!fileToSearch)
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
            {
                std::unique_lock lock(mutexQueue);
                this->searchedFiles++;
                addPathToQueue(entry.path());
                lock.unlock();
                emptyQueueCondition.notify_one();
            }
        }
    }
    finishedSearchingForFiles=true;
}

void ThreadPool::startWorkWithFile(){
    mutexThreadIdToPathsMap.lock();
    threadIdToPathsMap[std::this_thread::get_id()];
    mutexThreadIdToPathsMap.unlock();

    while(!finishedSearchingForFiles || !paths.empty()){
        fs::path pathToFile;
        {
            std::unique_lock lock(mutexQueue);

            while(paths.empty())
                emptyQueueCondition.wait(lock);

            pathToFile = paths.front();
            paths.pop();
        }
        searchWithinFile(pathToFile);
    }
}

void ThreadPool::searchWithinFile(fs::path &pathToFile){
    std::ifstream fileToSearch;
    fileToSearch.open(pathToFile);

    if (!fileToSearch)
        return;

    mutexThreadIdToPathsMap.lock();
    threadIdToPathsMap[std::this_thread::get_id()].push_back(pathToFile.filename());
    mutexThreadIdToPathsMap.unlock();

    bool isSearchedPatternFound = false;
    std::string line;
    unsigned int searchedWordIndex = 0;

    while (std::getline(fileToSearch, line)) {
        searchedWordIndex = line.find(stringToFind);

        if (searchedWordIndex != std::string::npos) {
            if (!isSearchedPatternFound) {
                isSearchedPatternFound = true;
                mutexFilesContainingPattern.lock();
                filesContainingPattern++;
                mutexFilesContainingPattern.unlock();
            }

            mutexPatternsNumber.lock();
            patternsNumber++;
            mutexPatternsNumber.unlock();

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

logDataMapType ThreadPool::getThreadIdToPathsMap() const{
    return threadIdToPathsMap;
}

resultDataMapType ThreadPool::getfilePathToLineMap() const {
    return filePathToLineMap;
}