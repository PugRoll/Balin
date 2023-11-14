#ifndef BALIN_CACHE_H
#define BALIN_CACHE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <memory>


struct ParsedData {
    bool c_compiler_valid;
    bool cpp_compiler_valid;
    bool includes_valid;
    /**
     * Some values will be stored as integers and hashed to save time  
     */
    unsigned int c_compiler;
    unsigned int cpp_compiler;
    unsigned int includes;
    unsigned int executable;
    unsigned int version;
    unsigned int flags;
};

bool checkforExistingCache();

void createCacheFile();
void writeToCacheFile(const std::string str, unsigned int hashValue);

void assignValueWithIdentifier(ParsedData &pd, const std::string identifier, unsigned int value); 
unsigned int getValueWithIdentifier(const std::string identifier);

ParsedData currentCacheData();

    

size_t balin_split(const std::string &line, std::vector<std::string> &str, const char delim);



#endif
