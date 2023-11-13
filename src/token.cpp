#include "../include/token.hpp"
#include "../include/balin_common.hpp"
#include <iostream>





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
            case hash("flags") :
                result = Token::Flag;
                break;
            case hash("debug") :
                result = Token::Debug;
                break;
            case hash("dependency") :
                result = Token::Dependency;
                break;
            case hash("library") :
                result = Token::Library;
                break;
            default:
                break;
        }
    }

    return result;
}

