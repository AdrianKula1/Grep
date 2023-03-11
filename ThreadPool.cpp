#include "ThreadPool.h"
#include<fstream>
ThreadPool::ThreadPool(long noThreads, std::string &stringToFind) {
    this->noThreads=noThreads;
    this->threads.reserve(noThreads);
    this->stringToFind=stringToFind;

    this->searchMutexes = new std::mutex[noThreads];
}

void ThreadPool::addPathToQueue(const fs::path& pathToFile){
    this->paths.push(pathToFile);
}

void ThreadPool::beginWork(){
    this->searchedFiles+=this->paths.size();

    for(int i=0; i<noThreads; i++)
        this->threads.emplace_back(&ThreadPool::fileSearcher, this, i);


    for(int i=0; i<noThreads; i++)
        this->threads[i].join();

}


void ThreadPool::fileSearcher(int id){
    logData[std::this_thread::get_id()];

    while(!paths.empty()){
        std::lock_guard<std::mutex> lock(searchMutexes[id]);

        fs::path pathToFile = paths.front();
        paths.pop();

        std::ifstream file;
        file.open(pathToFile);

        if(!file.good())
            continue;

        logData[std::this_thread::get_id()].push_back(pathToFile.filename());

        bool fileHasPattern = false;
        std::string line;
        unsigned int lineRow = 0, lineColumn = 0;

        while (std::getline(file, line)) {
            lineColumn = line.find(stringToFind);

            if (lineColumn != std::string::npos) {
                if (!fileHasPattern) {
                    fileHasPattern = true;
                    filesWithPattern++;
                }

                patternsNumber++;
                resultData[pathToFile].emplace_back(std::make_pair(lineColumn, line));
            }
            lineRow++;
        }
        file.close();
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