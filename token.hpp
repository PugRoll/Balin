#ifndef TOKEN_H
#define TOKEN_H

#include <string>

constexpr unsigned int hash(const char* str);

enum class Token{
    Version,
    Executable,
    CPP_Compiler,
    C_Compiler,
    Variable,
    Print,
    Unknown
};

Token getTokenValue(const std::string& token);



#endif // TOKEN_H
