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

#endif
