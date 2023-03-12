#ifndef TIETOEVRYTASK_GREP_H
#define TIETOEVRYTASK_GREP_H
#include<string>

#include "ThreadPool.h"

class Grep {
private:
    int minNumberOfArgsToRunTheProgram = 2;
    int argc;
    std::string *argv;

    std::string stringToFind, startDirectory, resultFileName, logFileName;
    long numberOfThreads;

    ThreadPool *threadPool;

    template<typename T>
    static bool compareData(T &a, T&b);

public:
    Grep(int args, char *argv[]);
    void main();
    void validateArguments() const;
    void setDefaultArguments();
    void setUserArguments();
    void createLogFile();
    void createResultFile();

    unsigned int getSearchedFiles() const;
    unsigned int getFilesWithPattern() const;
    unsigned int getPatternsNumber() const;
    std::string getResultFileName() const;
    std::string getLogFileName() const;
    unsigned int getNumberOfThreads() const;
};

#endif //TIETOEVRYTASK_GREP_H
