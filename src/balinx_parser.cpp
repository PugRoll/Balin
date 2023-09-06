#include "../include/balinx_parser.hpp"
#include "../include/token.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <algorithm>
#include <cctype>


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
                case Token::Variable :
                    success = addVariable(line);
                    break;
                case Token::Print : 
                    printMessage(line);
                    break;
                case Token::Include :
                    success = handle_includes(line);
                    break;
                case Token::Flag :
                    success = handle_flags(line);
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
        executable_name_ = resolveString(exec_name);
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
        c_compiler = resolveString(cc_name);
        return true; 
    }
    return false;
    
}

bool BalinxParser::handle_cpp_compiler(const std::string& line) {
    auto pos = line.find("cppc");
    if(pos != std::string::npos) {
        std::string cpp_name = line.substr(pos + 4);
        cpp_compiler = resolveString(cpp_name);
        return true; 
    }
    return false;
    
}

bool BalinxParser::addVariable(const std::string& line) {
    std::regex varRegex(R"(\$([a-zA-Z0-9]+)(?:\s+([^\$]+))?(?=\$|\s*$))");
    std::smatch varMatch;
    if(std::regex_search(line, varMatch, varRegex)) {
        std::string varIdent = varMatch[1];
        std::string varValue;
        if(varMatch.size() > 2) {
            varValue = varMatch[2];

            varValue.erase(varValue.begin(), std::find_if(varValue.begin(),  varValue.end(), [](int ch) {
                        return !std::isspace(ch);
            }));
            varValue.erase(std::find_if(varValue.rbegin() , varValue.rend(), [](int ch) {
                        return !std::isspace(ch);
            }).base(), varValue.end());

        }
        variables.push_back(std::make_pair(varIdent, varValue));
        return true;
    }
    return false;
}

bool BalinxParser::handle_includes(const std::string& line) {
    size_t pos = line.find("include");
    if(pos != std::string::npos) {
        std::string tmpInclude = line.substr(pos + 7);
        includes.push_back(resolveString(tmpInclude));
        return true;
    }
    return false;
}

bool BalinxParser::handle_flags(const std::string& line) {
    size_t pos = line.find("flags");
    if(pos != std::string::npos) {
        std::string tmpFlag = line.substr(pos + 5);
        tmpFlag = resolveString(tmpFlag);
        std::istringstream iss(tmpFlag);
        std::string flag;

        while(iss >> flag) {
            flags.push_back(flag);
        }

        return true;
    }
    return false;
}



void BalinxParser::printMessage(const std::string& line) {
    std::cout << resolveString(line) << "\n";
}

std::string BalinxParser::resolveString(const std::string& line) {
    size_t last = line.find_last_of("\"");
    size_t first = line.find("\"");
    std::string msg = "";
    if(first != std::string::npos && last != std::string::npos && last > first) {
        msg = line.substr(first + 1, last - first - 1);
        return msg;
        
    }
    else {
        std::cout << "ERROR\n";
    }
    return "";
}
