#include "../include/cache.hpp"
#include "../include/token.hpp"
#include <sstream>
#include <iostream>

const char COMMENT_CHARACTER = '#';
const char VALUE_SPLIT_CHARACTER = ':';
const char DELIMITER_CHARACTER = '>';    

ParsedData currentCache;

size_t balin_split(const std::string &line, std::vector<std::string> &str, const char delim) {
    size_t pos = line.find(delim);
    size_t initial_pos = 0;
    str.clear();
    while(pos != std::string::npos) {
        str.push_back(line.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;

        pos = line.find(delim, initial_pos);
    }
    str.push_back(line.substr(initial_pos, std::min(pos, str.size()) - initial_pos + 1));

    return str.size();
}

bool checkforExistingCache() {
    std::ifstream f("./build/cache.bx");
    return f.good(); 
}

void createCacheFile() {
    std::ofstream outfile("./build/cache.bx");
    outfile.close();
}


ParsedData currentCacheData() {
    ParsedData curr;

    if(!checkforExistingCache()){
        //If this branch is taken the cache file is not made and
        //therefore there is no cached data
        createCacheFile();
        return curr;
    }    

    std::ifstream file("./build/cache.bx");
    if(!file.is_open()) {
        balinError("File did not open is function: 'currentCacheData'");
    }
    std::string line;
    if(!std::getline(file, line)) {
        //false means that the file is empty
        return curr;
    }

    //File should be stored as one line
    std::istringstream iss(line);

    std::vector<std::string> strs;
    size_t sizeOfData = balin_split(line, strs, DELIMITER_CHARACTER); 

    std::vector<std::string> parts;
    size_t tmp;
    for(const std::string &str : strs) {
        tmp = balin_split(str, parts , VALUE_SPLIT_CHARACTER);
        if(tmp == 2) {
            assignValueWithIdentifier(curr, parts[0], hash(parts[1].c_str()));
        }
        std::ostringstream tmp;
        tmp << "Identifier: " << parts[0] << ", IntegerValue: " << parts[1];
        balinDebug(tmp);
        
    } 

    currentCache = curr;
    return curr;
}


void writeToCacheFile(const std::string str, unsigned int hashValue) {
    std::ofstream cacheOut;
    cacheOut.open("./build/cache.bx");
    if(cacheOut.is_open()) {
        cacheOut << str << VALUE_SPLIT_CHARACTER << hashValue << DELIMITER_CHARACTER;
        cacheOut.close();
    }
    else {
        balinError("Error opening cache.bx file");
    }
}


void assignValueWithIdentifier(ParsedData &pd, const std::string identifier, unsigned int value) {

    switch(hash(identifier.c_str())) {
        case hash("cc") :
            pd.c_compiler = value;
            break;
        case hash("cppc") :
            pd.cpp_compiler = value;
            break;
        case hash("includes") :
            pd.includes = value;
            break;
        default :
            balinError("Unknown identifier: " + identifier);
            break;
    }
}


unsigned int getValueWithIdentifier(const std::string identifier) {
    switch(hash(identifier.c_str())) {
            case hash("cc") :
                return currentCache.c_compiler;
                break;
            case hash("cppc") :
                return currentCache.cpp_compiler;
                break;
            case hash("includes") :
                return currentCache.includes;
                break;
            default :
                balinError("Unknown identifier: " + identifier);
                break;
        }
    return -1;
}




