#ifndef TIETOEVRYTASK_THREADPOOL_H
#define TIETOEVRYTASK_THREADPOOL_H
#include<string>
#include<thread>
#include<mutex>
#include<queue>
#include<map>
#include<filesystem>

namespace fs = std::filesystem;

class ThreadPool {
private:

    long noThreads;
    unsigned int searchedFiles=0, filesWithPattern=0, patternsNumber=0;

    std::string stringToFind;
    std::string startingDirectory;

    std::vector<std::thread> threads;
    std::thread directorySearcher;
    std::queue<fs::path> paths;

    std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>> resultData;
    std::map<std::thread::id, std::vector<fs::path>> logData;

    std::mutex *searchMutexes;
    std::mutex m;

public:

    ThreadPool(long noThreads, std::string& stringToFind);

    void addPathToQueue(const fs::path& pathToFile);

    void beginWork();

    void fileSearcher(int id);

    unsigned int getSearchedFiles() const;

    unsigned int getDilesWithPattern() const;

    unsigned int getPatternsNumber() const;

    std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>> getResultData() const;

    std::map<std::thread::id, std::vector<fs::path>> getLogData() const;

};


#endif //TIETOEVRYTASK_THREADPOOL_H
