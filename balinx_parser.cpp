#include "balinx_parser.hpp"
#include "token.hpp"
#include <fstream>
#include <sstream>
#include <iostream>






BalinxParser::BalinxParser(const std::string& filename) : file_(filename), filename_(filename) {}

bool BalinxParser::parse() {
    std::ifstream file = std::ifstream(BalinxParser::get_filename());
    if(!file.is_open()) {
        std::cerr << "Error: Build file could not be opened\n";
    }

    std::string line;
    while(std::getline(file, line)) {
        if(line.empty() || line.front() == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string token;
        if(std::getline(iss, token, ' ')) {

            bool success = false;
            std::string errorMsg = "";
            Token tk = getTokenValue(token);

            switch(tk) {
                case Token::Version :
                    success = handle_version(line);
                    break;
                case Token::Executable :
                    success = handle_executable(line);
                    break;
                case Token::C_Compiler :
                    success = handle_c_compiler(line);
                    break;
                case Token::CPP_Compiler :
                    success = handle_cpp_compiler(line);
                    break;
                //If token is found, assume the Token is unknown
                default :
                    break;

            }

        }
    }

    return true;
}

bool BalinxParser::handle_executable(const std::string& line) {
    size_t pos = line.find("executable"); 
    if(pos != std::string::npos) {
        std::string exec_name = line.substr(pos + 11);
        executable_name_ = exec_name;
        return true;
    }
    return false;
}



bool BalinxParser::handle_version(const std::string& line) {
    auto pos = line.find("version");
    if(pos != std::string::npos) {
        std::string version_str = line.substr(pos + 7);
        std::istringstream iss(version_str);
        std::string tk;
        while(std::getline(iss, tk, '.')) {
            int version_num;
            if(std::istringstream(tk) >> version_num) {
                version_number_.push_back(version_num);
            }
            else {
                return false;
            }
        }
    }
    return true;
}

bool BalinxParser::handle_c_compiler(const std::string& line) {
    auto pos = line.find("cc");
    if(pos != std::string::npos) {
        std::string cc_name = line.substr(pos + 2);
        c_compiler = cc_name;
        return true; 
    }
    return false;
    
}

bool BalinxParser::handle_cpp_compiler(const std::string& line) {
    auto pos = line.find("cppc");
    if(pos != std::string::npos) {
        std::string cpp_name = line.substr(pos + 4);
        cpp_compiler = cpp_name;
        return true; 
    }
    return false;
    
}








