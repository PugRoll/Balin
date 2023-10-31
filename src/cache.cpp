#include "../include/cache.hpp"
#include "../include/balin_common.hpp"
#include "../include/token.hpp"
#include <sstream>

const char COMMENT_CHARACTER = '#';
const char VALUE_SPLIT_CHARACTER = ':';
const char DELIMITER_CHARACTER = '>';    

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

bool checkforExistingCache() {
    std::ifstream f("../build/cache.bx");
    return f.good(); 
}

void createCacheFile() {
    std::ofstream outfile("../build/cache.bx");
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

    std::ifstream file("../build/cache.bx");
    std::string line;
    if(!std::getline(file, line)) {
        //false means that the file is empty
        return curr;
    }

    //File should be stored as one line
    std::istringstream iss(line);

    std::vector<std::string> strs;
    size_t sizeOfData = split(line, strs, DELIMITER_CHARACTER); 

    std::vector<std::string> parts;
    size_t tmp;
    for(const std::string &str : strs) {
        tmp = split(str, parts , VALUE_SPLIT_CHARACTER);
        if(tmp == 2) {
            assignValueWithIdentifier(curr, parts[0], hash(parts[1].c_str()));
        }
        
        std::cout << "\t[DEBUG] " << "Identifier: " << parts[0] << ", IntegerValue: " << parts[1] << "\r\n";
        
    } 


    return curr;
}


void writeToCacheFile(const std::string str, unsigned int hashValue) {
    std::ofstream cacheOut("../build/cache.bx");  
    cacheOut << str << VALUE_SPLIT_CHARACTER << hashValue << DELIMITER_CHARACTER;
    cacheOut.close();
}


void assignValueWithIdentifier(ParsedData *pd, const std::string identifier, unsigned int value) {

    switch(hash(identifier.c_str())) {
        case hash("cc") :
            pd->c_compiler = value;
            break;
        case hash("cppc") :
            pd->cpp_compiler = value;
            break;
        case hash("includes") :
            pd->includes = value;
            break;
        default :
            std::cerr << "\t[ERROR] Unknown identifier\r\n";
            break;
    }
}




