#ifndef TIETOEVRYTASK_GREP_H
#define TIETOEVRYTASK_GREP_H
#include<string>

class Grep {
private:
    int minNumberOfArgsToRunTheProgram = 2;
    int argc;
    std::string *argv;

    std::string STRING_TO_FIND, START_DIRECTORY, LOG_FILE_NAME, RESULT_FILE_NAME;
    long NUMBER_OF_THREADS;
public:
    Grep(int args, char *argv[]);
    void main();
    void validateArguments();
    void setDefaultArguments();
    void setUserArguments();
};


#endif //TIETOEVRYTASK_GREP_H
