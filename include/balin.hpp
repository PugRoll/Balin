#ifndef BALINX_H
#define BALINX_H

#include <string>
#include <vector>
#include "../include/balinx_parser.hpp"

class Balin {
public:
    explicit Balin(const std::string& filename);
    bool compile();



private: 


    bool setVars();

    std::ifstream file_;
    std::string filename_;
    std::string executable;
    std::string c_compiler;
    std::string cpp_compiler;
    std::vector<std::string> includes;
    std::vector<std::string> flags;
    BalinxParser parser;




};

#endif //BALINX_H
