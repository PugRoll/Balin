#ifndef BALIN_CACHE_H
#define BALIN_CACHE_H

#include <iostream>
#include <vector>
#include <fstream>

struct ParsedData;

bool checkforExistingCache();

void createCacheFile();
void writeToCacheFile(const std::string str, unsigned int hashValue);
void assignValueWithIdentifier(ParsedData &pd, const std::string identifier, unsigned int value); 

ParsedData currentCacheData();

    

size_t balin_split(const std::string &line, std::vector<std::string> &str, const char delim);



#endif
