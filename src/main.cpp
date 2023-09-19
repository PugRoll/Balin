#include "../include/balin.hpp"
#include <iostream> 
#include <getopt.h>

int main(int argc, char* argv[]) {
    bool debugMode = false;
    std::string filename = "build.bx";
    int opt;
    while((opt = getopt(argc, argv, "d")) != -1) {
        switch(opt) {
            case 'd': 
                debugMode = true;
                break;
        }

    }
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
