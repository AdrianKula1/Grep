#ifndef TIETOEVRYTASK_THREADPOOL_H
#define TIETOEVRYTASK_THREADPOOL_H
#include<string>
#include<thread>
#include<mutex>
#include<queue>
#include<map>
#include<filesystem>
#include<condition_variable>

namespace fs = std::filesystem;

class ThreadPool {
private:
    long noThreads;
    unsigned int searchedFiles=0, filesContainingPattern=0, patternsNumber=0;

    std::string stringToFind;
    std::string startingDirectory;
    std::string resultFileName;
    std::string startDirectory;

    std::vector<std::thread> threads;
    std::thread directorySearcher;
    std::queue<fs::path> paths;

    std::map<std::thread::id, std::vector<fs::path>> threadIdToPathsMap;

    std::mutex queueMutex;
    std::mutex resultFileMutex;

    std::condition_variable emptyQueueCondition;

    bool allFilesFound=false;

public:

    ThreadPool(long noThreads, std::string& stringToFind, std::string& resultFileName, std::string& startDirectory);
    void resetResultFile();
    void searchDirectory();
    void addPathToQueue(const fs::path& pathToFile);
    void beginWork();
    void startWorkWithFile();
    void searchWithinFile(fs::path &pathToFile);
    void saveLineToResultFile(fs::path &pathToFile, unsigned int searchedWordIndex, std::string& line);
    unsigned int getSearchedFiles() const;
    unsigned int getFilesWithPattern() const;
    unsigned int getPatternsNumber() const;
    std::map<std::thread::id, std::vector<fs::path>> getThreadIdToPathsMap() const;

};


#endif //TIETOEVRYTASK_THREADPOOL_H
