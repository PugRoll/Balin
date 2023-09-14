#include <iostream>
#include "../include/balin.hpp"


#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << "failed on line: " << __LINE__ << std::endl; }
#define UNIT_TEST(x, expected) { std::cout << "\t[TEST] " <<  __FUNCTION__ << " Actual: " << x << " Expected: " << expected << std::endl; }


bool finalCheck(bool c_compiler, bool cpp_compiler, bool validC, bool validCPP) {

    //Check to make sure they are empty strings
    bool c_isNotEmpty = c_compiler;
    bool cpp_isNotEmpty = cpp_compiler;

    if(c_isNotEmpty && cpp_isNotEmpty) {
        return true;
    }

    //More specific checks
    if(!(c_isNotEmpty && cpp_isNotEmpty)) { //Both string values are empty
        return false;
    }
    else {
        if(!c_isNotEmpty && cpp_isNotEmpty && validCPP) {
            return true;
        }
        if(c_isNotEmpty && validC && !cpp_isNotEmpty) {
            return false;
        }
    }

    return false;

}


void printMsg_NoNL(const char* str) {
    std::cout << str;
}
void printMsg(const char* str) {
    std::cout << str << std::endl;
}


void test_finalCheck() {
    const char* line = "--------------------------------------";
    printMsg("All true and All false:");
    printMsg(line);
    UNIT_TEST(finalCheck(false, false, false, false), false);
    UNIT_TEST(finalCheck(true, true, true, true), true);

    printMsg("One true value");
    printMsg(line);
    UNIT_TEST(finalCheck(true, false, true, true), true);
    UNIT_TEST(finalCheck(false, true, true, true), true);
    

}


int main() {
    test_finalCheck();

    return 0;

}
