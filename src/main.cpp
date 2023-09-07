#include "../include/balin.hpp"
#include <iostream> 

int main(int argc, char* argv[]) {
    std::string filename = "build.bx";
    if(argc > 1 ){
        filename = argv[1];
    }

    Balin balin(filename);

    if(balin.compile()) {
        return 0;
        std::cout << "SUCCESS\n";
    }
    else {
        std::cout << "FAILURE\n";
        return 1;
    }


    
    
    
}
