#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "../include/balin_common.hpp"


enum class Token{
    Version,
    Executable,
    CPP_Compiler,
    C_Compiler,
    Variable,
    Include,
    Library,
    Before, //tasks
    After, //tasks 
    Flag,
    Print,
    Debug,
    Dependency,
    Unknown
};

/**
 * Functionalilty: return the enumerated value of Token
 * @Param: string token -> the string version of token
 */
Token getTokenValue(const std::string& token);



#endif // TOKEN_H
