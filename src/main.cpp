#include "../include/balin.hpp"
#include "../include/balin_common.hpp"
#include <iostream> 
#include <getopt.h>



int main(int argc, char* argv[]) {
    initFlags();
    std::string filename = "build.bx";
    int opt;
    while((opt = getopt(argc, argv, "dieq")) != -1) {
        switch(opt) {
            case 'd': 
                setDebugFlag(true);
                break;
            case 'i' :
                setInfoFlag(true);
                break;
            case 'q' :
                // quiet option
                setDebugFlag(false);
                setInfoFlag(false);
                break;
            case 'e' : //Using 'e' for development purposes
                setDevDebugFlag(true);
                setDebugFlag(true);
                setInfoFlag(true);
                break;
            default :
                balinError("Invalid option");
                return 1;

        }

    }
    for (int i = optind; i < argc; ++i) {
        filename = argv[i];
    }

    Balin balin(filename);

    if(balin.compile()) {
        balinSuccess();
        return 0;
    }
    else {
        balinFailure("Reason for failure unsure");
        return 1;
    }
    
    
}
