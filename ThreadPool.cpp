#include "ThreadPool.h"
#include<fstream>
ThreadPool::ThreadPool(long noThreads, std::string &stringToFind) {
    this->noThreads=noThreads;
    this->threads.reserve(noThreads);
    this->stringToFind=stringToFind;

}

void ThreadPool::addPathToQueue(const fs::path& pathToFile){
    this->paths.push(pathToFile);
}

void ThreadPool::beginWork(){
    this->searchedFiles+=this->paths.size();

    for(int i=0; i<noThreads; i++)
        this->threads.emplace_back(&ThreadPool::fileWorker, this, i);


    for(int i=0; i<noThreads; i++)
        this->threads[i].join();

}

void ThreadPool::saveResultToFile(fs::path &pathToFile, unsigned int lineColumn, std::string& line){
    std::ofstream resultFile;
    resultFile.open(pathToFile);
    if(!resultFile.good() || !resultFile.is_open())
        return;

    resultFile << pathToFile.string() << ':' << lineColumn << ": " << line << std::endl;
}

void ThreadPool::searchFile(fs::path &pathToFile){
    std::ifstream fileToSearch;
    fileToSearch.open(pathToFile);

    if (!fileToSearch.good() || !fileToSearch.is_open())
        return;

    logData[std::this_thread::get_id()].push_back(pathToFile.filename());

    bool fileHasPattern = false;
    std::string line;
    unsigned int lineRow = 0, lineColumn = 0;

    while (std::getline(fileToSearch, line)) {
        lineColumn = line.find(stringToFind);

        if (lineColumn != std::string::npos) {
            if (!fileHasPattern) {
                fileHasPattern = true;
                filesWithPattern++;
            }
            patternsNumber++;
            saveResultToFile(pathToFile, lineColumn, line);
        }
        lineRow++;
    }
    fileToSearch.close();
}

void ThreadPool::fileWorker(){
    logData[std::this_thread::get_id()];

    while(!paths.empty()){
        {
            std::lock_guard<std::mutex> lock(queueMutex);

            fs::path pathToFile = paths.front();
            paths.pop();

            searchFile(pathToFile);
        }
    }
}

unsigned int ThreadPool::getSearchedFiles() const{
    return this->searchedFiles;
}

unsigned int ThreadPool::getDilesWithPattern() const{
    return this->filesWithPattern;
}

unsigned int ThreadPool::getPatternsNumber() const{
    return this->patternsNumber;
}

std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>> ThreadPool::getResultData() const{
    return resultData;
}
std::map<std::thread::id, std::vector<fs::path>> ThreadPool::getLogData() const{
    return logData;
}