#include "ThreadPool.h"
#include<fstream>
#include<iostream>

ThreadPool::ThreadPool(long newNoThreads, std::string &newStringToFind, std::string &newResultFileName, std::string& newStartDirectory) {
    noThreads=newNoThreads;
    threads.reserve(noThreads);

    stringToFind=newStringToFind;
    resultFileName=newResultFileName;
    startDirectory=newStartDirectory;

    searchedFiles=0;
    filesContainingPattern=0;
    patternsNumber=0;

    resetResultFile();
}

void ThreadPool::resetResultFile() {
    std::ofstream fileToSearch;
    fileToSearch.open(resultFileName);

    if (!fileToSearch)
        return;

    fileToSearch.close();
}

void ThreadPool::beginWork(){
    directorySearcher = std::thread(&ThreadPool::searchDirectory, this);

    for(int i=0; i<noThreads; i++)
        threads.emplace_back(&ThreadPool::startWorkWithFile, this);

    directorySearcher.join();

    for(int i=0; i<noThreads; i++)
        threads[i].join();
}

void ThreadPool::searchDirectory() {
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(startDirectory)){
        if(entry.is_regular_file()){
            {
                searchedFiles++;
                std::unique_lock<std::mutex> lock(mutexQueue);
                paths.push(entry.path());
                lock.unlock();
                emptyQueueCondition.notify_all();
            }
        }
    }
    finishedSearchingForFiles=true;
}

void ThreadPool::startWorkWithFile(){
    {
        std::lock_guard<std::mutex> threadIdToPathsMapGuard(mutexThreadIdToPathsMap);
        threadIdToPathsMap[std::this_thread::get_id()];
    }

    while(!finishedSearchingForFiles && !paths.empty()){
        fs::path pathToFile;
        {
            std::unique_lock<std::mutex> queueLock(mutexQueue);

            while(paths.empty())
                emptyQueueCondition.wait(queueLock);

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

    {
        std::lock_guard<std::mutex> threadIdToPathsMapGuard(mutexThreadIdToPathsMap);
        threadIdToPathsMap[std::this_thread::get_id()].push_back(pathToFile.filename());
    }

/*    bool isSearchedPatternFound = false;
    std::string line;
    unsigned int searchedPhraseIndex = 0;

    while (std::getline(fileToSearch, line)) {
        searchedPhraseIndex = line.find(stringToFind);
        bool phraseFound = searchedPhraseIndex != std::string::npos;

        if (phraseFound && !isSearchedPatternFound) {
            isSearchedPatternFound = true;

            std::lock_guard<std::mutex> filesContainingPatternGuard(mutexFilesContainingPattern);
            filesContainingPattern++;
        }

        if (phraseFound) {
            {
                std::lock_guard<std::mutex> patternsNumberGuard(mutexPatternsNumber);
                patternsNumber++;
            }
            {
                std::lock_guard<std::mutex> filePathToLineMapGuard(mutexFilePathToLineMap);
                filePathToLineMap[pathToFile].emplace_back(searchedPhraseIndex, line);
            }
        }
    }*/
    fileToSearch.close();
}

void ThreadPool::addPathToQueue(const fs::path& pathToFile){
    paths.push(pathToFile);
}

unsigned int ThreadPool::getSearchedFiles() const{
    return searchedFiles;
}

unsigned int ThreadPool::getFilesWithPattern() const{
    return filesContainingPattern;
}

unsigned int ThreadPool::getPatternsNumber() const{
    return patternsNumber;
}

logDataMapType ThreadPool::getThreadIdToPathsMap() const{
    return threadIdToPathsMap;
}

resultDataMapType ThreadPool::getfilePathToLineMap() const {
    return filePathToLineMap;
}