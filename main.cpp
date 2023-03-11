#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
#include <queue>
#include <map>

#include "Grep.h"

namespace fs=std::filesystem;






int main(int argc, char *argv[]){
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();



    Grep g(argc, argv);
    g.main();


    //threadPool pool(NUMBER_OF_THREADS, STRING_TO_FIND);
/*
    for(const fs::directory_entry& entry : fs::recursive_directory_iterator(START_DIRECTORY)){
        if(entry.is_regular_file()){
            pool.addPathToQueue(entry.path());
        }
    }

    pool.beginWork();*/
/*
    unsigned int searchedFiles = pool.getSearchedFiles();
    unsigned int filesWithPattern = pool.getDilesWithPattern();
    unsigned int patternsNumber = pool.getPatternsNumber();

    std::map<fs::path, std::vector<std::pair<unsigned int, std::string>>> resultData = pool.getResultData();
    std::map<std::thread::id, std::vector<fs::path>> logData = pool.getLogData();

    std::ofstream resultFile(RESULT_FILE_NAME);
    if(resultFile.is_open()){
        for(auto &path: resultData){
            for (auto &numberAndLine: path.second){
                resultFile << path.first.string() << ':' << numberAndLine.first << ": " << numberAndLine.second << std::endl;
            }
        }
    }
    resultFile.close();


    std::ofstream logFile(LOG_FILE_NAME);
    if(logFile.is_open()){
        for(auto &threadData: logDataVector){
            logFile << threadData.first << ": ";
            for (auto &filename: threadData.second){
                logFile << filename << ',' ;
            }
            logFile << std::endl;
        }
    }
    logFile.close();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long int elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cout << "Searched files: " << searchedFiles << std::endl;
    std::cout << "Files with pattern: " << filesWithPattern << std::endl;
    std::cout << "Patterns number: " << patternsNumber << std::endl;
    std::cout << "Result file: " << RESULT_FILE_NAME << std::endl;
    std::cout << "Log file: " << LOG_FILE_NAME << std::endl;
    std::cout << "Used threads: " << NUMBER_OF_THREADS << std::endl;
    std::cout << "Elapsed time: " << elapsedTime  << "[ms]"<< std::endl;*/

    return 0;
}
