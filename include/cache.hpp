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

/** 
 * Functionalilty: Make sure their is an existing cache file
 */
bool checkforExistingCache();

/**
 *  Functionalilty: Create the cache file
 */
void createCacheFile();

/**
 * Functionalilty: Write some data to the cache file
 * @Param: string str -> a string value to write to file
 * @Param: integer hashValue -> a hash value also written to a file
 */
void writeToCacheFile(const std::string str, unsigned int hashValue);

/**
 * Functionalilty: assign a value with the given identifer
 * @Param: ParsedData &pd -> current parsedData structure
 * @Param: string identifier -> the identifier in question
 * @Param: integer value -> hash value
 */
void assignValueWithIdentifier(ParsedData &pd, const std::string identifier, unsigned int value); 

/**
 * Functionalilty: return a value with an identifer
 * @Param: string identifer -> the identifer to get the value
 *
 */
unsigned int getValueWithIdentifier(const std::string identifier);

/**
 * Functionalilty: return the current cache data from the cache file
 */
ParsedData currentCacheData();

    

/**
 * Functionalilty: split a string with a delimeter
 * @Param: string& line -> line to split
 * @Param: vector of strings &str -> passed by reference vector of strings
 * @Param: constant character delimter -> the delimiting character
 */
size_t balin_split(const std::string &line, std::vector<std::string> &str, const char delim);



#endif
