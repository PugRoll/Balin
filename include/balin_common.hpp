#ifndef BALIN_COMMON_H 
#define BALIN_COMMON_H
#include <iostream>
#include <vector>
#include <sstream>


namespace balin_color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[091m";
    const std::string OKGREEN = "\033[092m";
    const std::string YELLOW = "\033[093m";
    const std::string OKBLUE = "\033[094m";
}



//size_t balin_split(const std::string line, std::vector<std::string> &str, const char delim);

/**
 * Functionalilty: return a simple hash value
 * @Param: char* str -> C style string to hash
 */
constexpr unsigned int hash(const char* str)  {
    unsigned int hashValue = 0;
    while(*str) {
        hashValue = (hashValue << 5) + *str++;
    }
    return hashValue;
}

struct balin_flags {
    bool infoFlag;
    bool debugFlag;
} extern bFlags;

std::string resolveString(const std::string& line);

void balinSuccess();
void balinFailure(const char* str);

void balinInfo(const char* str);
void balinDebug(const char* str);
void balinError(const char* error);

void balinInfo(const std::string& str);
void balinDebug(const std::string& str);
void balinError(const std::string& error);

void balinInfo(const std::ostringstream& str);
void balinDebug(const std::ostringstream& str);
void balinError(const std::ostringstream& error);

void setInfoFlag(bool value);
void setDebugFlag(bool value);

void initFlags();


#endif
