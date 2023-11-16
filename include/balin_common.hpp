#ifndef BALIN_COMMON_H 
#define BALIN_COMMON_H
#include <iostream>
#include <vector>

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
}extern bFlags;



void balinInfo(const char* str);
void balinDebug(const char* str);
void balinError(const char* error);

void setInfoFlag(bool value);
void setDebugFlag(bool value);

void initFlags();


#endif
