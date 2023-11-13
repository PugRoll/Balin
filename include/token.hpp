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
    Flag,
    Print,
    Debug,
    Dependency,
    Unknown
};

Token getTokenValue(const std::string& token);



#endif // TOKEN_H
