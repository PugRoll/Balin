#ifndef BALINX_H
#define BALINX_H

#include <string>
#include <vector>
#include "../include/balinx_parser.hpp"

class Balin {
public:
    explicit Balin(const std::string& filename);
    bool compile();
    void processFile();
    const std::string& get_filename() const {return filename_; }



private: 
    bool setVars();
    bool testCompiler(const std::string& compiler, const std::string lang);
    bool finalCheck();
    std::string substituteVars(const std::string input, const std::vector<std::pair<std::string, std::string>>& vars);
    
    void unzipArchive(const char* depName);

    void createBuildDirectory();
    bool checkBuildDirectory();

    void createCacheFile();
    bool checkForCacheFile();

    bool checkDependencies();
    bool checkAgainstDependencyList(const std::string dep);
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

    bool doCompile;
    bool validCCompiler;
    bool validCppCompiler;







};

#endif //BALINX_H
