#ifndef TIETOEVRYTASK_GREP_H
#define TIETOEVRYTASK_GREP_H
#include<string>

#include "ThreadPool.h"

class Grep {
private:
    using resultDataVectorType = std::vector<std::pair<fs::path, std::vector<std::pair<unsigned int, std::string>>>>;
    using resultDataVectorItemType = std::pair<fs::path, std::vector<std::pair<unsigned int, std::string>>>;

    using logDataVectorType = std::vector<std::pair<std::thread::id, std::vector<fs::path>>>;
    using logDataVectorItemType = std::pair<std::thread::id, std::vector<fs::path>>;

    int minNumberOfArgsToRunTheProgram;
    int argc;
    std::string *argv;

    std::string stringToFind;
    std::string startDirectory;
    std::string resultFileName;
    std::string logFileName;

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
