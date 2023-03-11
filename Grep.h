#ifndef TIETOEVRYTASK_GREP_H
#define TIETOEVRYTASK_GREP_H
#include<string>
#include "ThreadPool.h"
class Grep {
private:
    int minNumberOfArgsToRunTheProgram = 2;
    int argc;
    std::string *argv;

    std::string STRING_TO_FIND, START_DIRECTORY, RESULT_FILE_NAME, LOG_FILE_NAME;
    long NUMBER_OF_THREADS;

    ThreadPool *threadPool;

    static bool compareLogData(std::pair<std::thread::id, std::vector<fs::path>>& a,
                               std::pair<std::thread::id, std::vector<fs::path>>& b)
    {
        return a.second.size() > b.second.size();
    }

public:
    Grep(int args, char *argv[]);
    void main();
    void validateArguments();
    void setDefaultArguments();
    void setUserArguments();
    void searchDirectory();
    void createLogFile();

    unsigned int getSearchedFiles() const;
    unsigned int getFilesWithPattern() const;
    unsigned int getPatternsNumber() const;

    std::string getResultFileName() const;
    std::string getLogFileName() const;
    unsigned int getNumberOfThreads() const;
};


#endif //TIETOEVRYTASK_GREP_H
