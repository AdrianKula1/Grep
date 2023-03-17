#ifndef TIETOEVRYTASK_THREADPOOL_H
#define TIETOEVRYTASK_THREADPOOL_H
#include<condition_variable>
#include<filesystem>
#include<string>
#include<thread>
#include<mutex>
#include<queue>
#include<map>

namespace fs = std::filesystem;

using logDataMapType = std::map<std::thread::id, std::vector<fs::path>>;
using resultDataMapType = std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>>;

class ThreadPool {
private:
    long noThreads;
    unsigned int searchedFiles;
    unsigned int filesContainingPattern;
    unsigned int patternsNumber;

    std::string stringToFind;
    std::string startingDirectory;
    std::string resultFileName;
    std::string startDirectory;

    std::vector<std::thread> threads;

    std::mutex mutexQueue;
    std::mutex mutexFilePathToLineMap;
    std::mutex mutexThreadIdToPathsMap;
    std::mutex mutexFilesContainingPattern;
    std::mutex mutexPatternsNumber;

    std::queue<fs::path> paths;

    std::condition_variable emptyQueueCondition;

    //Maps used for storing results and logs
    resultDataMapType filePathToLineMap;
    logDataMapType threadIdToPathsMap;


public:

    ThreadPool(long newNoThreads, std::string& newStringToFind, std::string& newResultFileName, std::string& newStartDirectory);
    void resetResultFile();
    void searchDirectory();
    void beginWork();
    void startWorkWithFile();
    void searchWithinFile(fs::path &pathToFile);

    unsigned int getSearchedFiles() const;
    unsigned int getFilesWithPattern() const;
    unsigned int getPatternsNumber() const;
    logDataMapType getThreadIdToPathsMap() const;
    resultDataMapType getfilePathToLineMap() const;

};

#endif //TIETOEVRYTASK_THREADPOOL_H
