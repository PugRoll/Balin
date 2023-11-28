#ifndef BALINX_H
#define BALINX_H

#include <string>
#include <vector>
#include "../include/balinx_parser.hpp"

class Balin {
public:
    explicit Balin(const std::string& filename);

    /**
     * Functionalilty: Start and perform the compilation process
     */
    bool compile();

    /**
     * Functionalilty: process the ".bx" file
     */
    void processFile();

    /**
     * Functionalilty: return the current filename being processed 
     */
    const std::string& get_filename() const {return filename_; }



private: 
    /**
     * Functionalilty: Set the variables returned from the parser
     */
    bool setVars();
    /** 
     * Functionalilty: ensure that the compiler passed in is valid and can return a version number
     * @Param: string& compiler -> the compiler that was parsed from .bx file
     * @Param: string lang -> the language in question either "C" or "C++"
     */
    bool testCompiler(const std::string& compiler, const std::string lang);
    /**
     * Functionalilty: Perform the final check to ensure we can compile
     */
    bool finalCheck();
    /**
     * Functionalilty: parse through and substitue variables where applicable
     * @Param: string input -> the line being parsed
     * @Param: Vector containing pairs of strings vars -> the variables parsed from the file
     */
    std::string substituteVars(const std::string input, const std::vector<std::pair<std::string, std::string>>& vars);
    
    /**
     * Functionalilty: unzip the archive file 
     * @Param: char* depName -> dependency name. will be unzipped into build file.
     */
    void unzipArchive(const char* depName);

    /**
     * Functionalilty: Create the build directory
     */
    void createBuildDirectory();
    /**
     * Functionalilty: check for valid build directory
     */
    bool checkBuildDirectory();

    /**
     * Functionalilty: Create the cache file
     */

    void createCacheFile();
    /**
     * Functionalilty: Check for an existing cache file
     */
    bool checkForCacheFile();
    
    /**
     * Functionalilty: check the cache file with the hash of the current cache database
     * @Param: string curr -> current data in question
     */
    bool checkWithCache(const std::string curr);

    /**
     * Functionalilty: check the dependencies
     */
    bool checkDependencies();

    /**
     * Functionalilty: run through the dependency list, make sure it is valid dependency
     */
    bool checkAgainstDependencyList(const std::string dep);


    /**
     * Functionalilty: perform compilation
     */
    bool doCompile;

    bool doBefore();

    /*************************************************************************************
     * Variables:
     *
     */

    bool validCCompiler;
    bool validCppCompiler;

    std::ifstream file_;
    std::ofstream cacheFile;
    std::string filename_;
    std::string executable;
    std::string c_compiler;
    std::string cpp_compiler;
    std::vector<std::string> includes;
    std::vector<std::string> flags;
    std::vector<std::string> debugs;
    std::vector<std::string> deps;
    std::vector<std::string> libs;

    BalinxParser parser;


};

#endif //BALINX_H
