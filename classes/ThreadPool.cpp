#include "ThreadPool.h"
#include<fstream>

ThreadPool::ThreadPool(long newNoThreads, std::string &newStringToFind, std::string &newResultFileName, std::string& newStartDirectory) {
    noThreads = newNoThreads;
    threads.reserve(noThreads);

    stringToFind = newStringToFind;
    resultFileName = newResultFileName;
    startDirectory = newStartDirectory;

    searchedFiles = 0;
    filesContainingPattern = 0;
    patternsNumber = 0;

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
    searchDirectory();

    for(int i=0; i<noThreads; i++)
        threads.emplace_back(&ThreadPool::startWorkWithFile, this);

    for(int i=0; i<noThreads; i++)
        threads[i].join();
}

void ThreadPool::searchDirectory() {
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(startDirectory)){
        if(entry.is_regular_file()){
            searchedFiles++;
            paths.push(entry.path());
        }
    }
}

void ThreadPool::startWorkWithFile(){
    {
        std::lock_guard<std::mutex> threadIdToPathsMapGuard(mutexThreadIdToPathsMap);
        threadIdToPathsMap[std::this_thread::get_id()];
    }

    while(!paths.empty()){
        fs::path pathToFile;
        {
            std::unique_lock<std::mutex> queueLock(mutexQueue);
            if(!paths.empty()) {
                pathToFile = paths.front();
                paths.pop();
            }
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

    bool isSearchedPatternFound = false;
    std::string line;
    unsigned int searchedPhraseIndex = 0;

    while (std::getline(fileToSearch, line)) {
        searchedPhraseIndex = line.find(stringToFind, 0);
        bool phraseFound = searchedPhraseIndex != std::string::npos;

        if (phraseFound && !isSearchedPatternFound) {
            isSearchedPatternFound = true;

            std::lock_guard<std::mutex> filesContainingPatternGuard(mutexFilesContainingPattern);
            filesContainingPattern++;
        }

        while (searchedPhraseIndex != std::string::npos) {
            {
                std::lock_guard<std::mutex> patternsNumberGuard(mutexPatternsNumber);
                patternsNumber++;
            }
            {
                std::lock_guard<std::mutex> filePathToLineMapGuard(mutexFilePathToLineMap);
                filePathToLineMap[pathToFile].emplace_back(searchedPhraseIndex, line);
            }
            searchedPhraseIndex = line.find(stringToFind, searchedPhraseIndex+1);
        }
    }
    fileToSearch.close();
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