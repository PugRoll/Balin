#include "../include/balinx_parser.hpp"
#include "../include/token.hpp"
#include "../include/balin_common.hpp"
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
        balinError("Error: Build file could not be opened");
        return false;
    }

    std::string line;
    unsigned int currLine = 0;
    while(std::getline(file, line)) {
        currLine += 1;
        if(line.empty() || line.front() == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string token;
        if(std::getline(iss, token, ' ')) {

            bool success = false;
            std::string errorMsg = "";
            Token tk = getTokenValue(token);
            balinDevDebug("Current token: " + token);

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
                case Token::Debug :
                    success = debug_file(line);
                    break;
                case Token::Dependency :
                    success = add_dependency(line);
                    break;
                case Token::Library :
                    success = add_library(line);
                    break;
                case Token::Before : { 
                    std::fstream f(BalinxParser::get_filename());
                    balin_gotoLine(f, currLine);
                    std::string beforeLine;
                    std::getline(f, beforeLine);
                    success = handle_before_ident(beforeLine);
                    testBefore(beforeTasks);
                    break;
                }
                case Token::After : 
                    success = true;
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

bool BalinxParser::debug_file(const std::string& line) {
    size_t pos = line.find("debug");
    if(pos != std::string::npos) {
        std::string tmpFilename = line.substr(pos + 5);
        addDebugFlags.push_back(resolveString(tmpFilename));
        return true;
    }
    balinError("Error in debug_file");
    return false;
}



void BalinxParser::printMessage(const std::string& line) {
    std::cout << resolveString(line) << "\n";
}



bool BalinxParser::add_dependency(const std::string& line) {
    const char* tmp = "dependency";
    size_t pos = line.find("dependency");
    if(pos != std::string::npos) {
        std::string dep = line.substr(pos + sizeof(*tmp));
        deps.push_back(resolveString(dep));
        return true;
    }
    balinError("Error in dependency function");
    return false;
}


bool BalinxParser::add_library(const std::string& line) {
    const char* tmp = "library";
    size_t pos = line.find("library");
    if(pos != std::string::npos) {
        std::string lib = line.substr(pos + sizeof(*tmp));
        libs.push_back(resolveString(lib));
        return true;
    }
    balinError("Error in Library function");
    return false;

}



bool BalinxParser::handle_before_ident(const std::string& line) {
    const char OPEN_BRACE = '{';
    const char CLOSE_BRACE = '}';
    bool insideBeforeBlock = false;
    std::stack<char> braces;

    std::ostringstream open_gamer;
    open_gamer << "before " << OPEN_BRACE;

    std::string ln;
    std::fstream f = std::fstream(BalinxParser::get_filename());

    while(std::getline(f,ln)) {
       size_t pos = ln.find(open_gamer.str());
       if(pos != std::string::npos) {
           insideBeforeBlock = true;
           braces.push(OPEN_BRACE);
           ln.erase(pos);
           if(line.empty()) continue;
       }

       if(insideBeforeBlock) {
           for(char ch : ln) {
               if(ch == OPEN_BRACE) {
                    braces.push(OPEN_BRACE);
               }
               else if (ch == CLOSE_BRACE) {
                   if(!braces.empty() && braces.top() == OPEN_BRACE) {
                       braces.pop();
                       if(braces.empty()) {
                           insideBeforeBlock = false;
                           //break; //Exit loop, end of block found
                       }
                   } 
                   else {
                       //Brace mismatch Error
                       balinError("Brace mismatch Errror LINE:273");
                       return false;
                   }
               }
           }

           if(insideBeforeBlock) {
               beforeTasks.push_back(ln);
           }
       }
    }

    if(!braces.empty()) {
        balinError("Brace mismatch error LINE:288");
        return false;
    }

    return true;
}



void BalinxParser::testBefore(std::vector<std::string> v) {
    for(const auto& line : v) {
        balinDebug("[In testBefore]: " + line);
    }
}
