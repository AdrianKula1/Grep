#include <iostream>

#include "Grep.h"

namespace fs=std::filesystem;

void displayResults(Grep *g, long long int elapsedTime){
    std::cout << "Searched files: " << g->getSearchedFiles() << std::endl;
    std::cout << "Files with pattern: " << g->getFilesWithPattern() << std::endl;
    std::cout << "Patterns number: " << g->getPatternsNumber() << std::endl;
    std::cout << "Result file: " << g->getResultFileName() << std::endl;
    std::cout << "Log file: " << g->getLogFileName() << std::endl;
    std::cout << "Used threads: " << g->getNumberOfThreads() << std::endl;
    std::cout << "Elapsed time: " << elapsedTime  << "[ms]"<< std::endl;
}

int main(int argc, char *argv[]){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Grep *g;

    try{
        g=new Grep(argc, argv);
    }catch (std::exception &e){
        std::cout << e.what() << std::endl;
        return 0;
    };

    g->main();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    displayResults(g, elapsedTime);

    return 0;
}
