#include "../include/token.hpp"
#include <iostream>

constexpr unsigned int hash(const char* str)  {
    unsigned int hashValue = 0;
    while(*str) {
        hashValue = (hashValue << 5) + *str++;
    }

    return hashValue;
}

Token getTokenValue(const std::string& token) {
    Token result = Token::Unknown;
    if(token.empty() || token[0] == '$') {
        result = Token::Variable;
    }
    else {
        switch(hash(token.c_str())) {
            case hash("version") :
                result = Token::Version;
                break;
            case hash("executable") :
                result = Token::Executable;
                break;
            case hash("cc") :
                result = Token::C_Compiler;
                break;
            case hash("cppc") :
                result = Token::CPP_Compiler;
                break;
            case hash("print") :
                result = Token::Print;
                break;
            case hash("include") :
                result = Token::Include;
                break;
            default:
                break;
        }
    }

    return result;
}

