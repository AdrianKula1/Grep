#include <iostream>
#include <memory>

#include "Grep.h"

namespace fs=std::filesystem;

void displayResults(std::unique_ptr<Grep> &grep, long long int elapsedTime){
    std::cout << "Searched files: " << grep->getSearchedFiles() << std::endl;
    std::cout << "Files with pattern: " << grep->getFilesWithPattern() << std::endl;
    std::cout << "Patterns number: " << grep->getPatternsNumber() << std::endl;
    std::cout << "Result file: " << grep->getResultFileName() << std::endl;
    std::cout << "Log file: " << grep->getLogFileName() << std::endl;
    std::cout << "Used threads: " << grep->getNumberOfThreads() << std::endl;
    std::cout << "Elapsed time: " << elapsedTime  << "[ms]"<< std::endl;
}

int main(int argc, char *argv[]){
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::unique_ptr<Grep> grep;

    try{
        grep = std::make_unique<Grep>(argc, argv);
    }catch (std::exception &e){
        std::cout << e.what() << std::endl;
        return 0;
    };

    grep->main();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    displayResults(grep, elapsedTime);

    return 0;
}
