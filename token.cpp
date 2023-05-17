#include "token.hpp"

constexpr unsigned int hash(const char* str)  {
    unsigned int hashValue = 0;
    while(*str) {
        hashValue = (hashValue << 5) + *str++;
    }

    return hashValue;
}

Token getTokenValue(const std::string& token) {
    Token result = Token::Unknown;
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
            default:
                break;
        }

    return result;
}

