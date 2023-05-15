#include "balinx_parser.hpp"
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
            std::cout << "Current Token: " << token << "\n";

            if(token == "version") {
                if(handle_version(line)) {
                    std::cout << "Success: Version number\n";
                }
                else {
                    std::cout << "Error: Version number\n";
                }
            }
            else if (token == "executable") {
                if(handle_executable(line)) {
                    std::cout << "Success: executable\n";
                }
                else {
                    std::cout << "Error: executable\n";
                }
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



