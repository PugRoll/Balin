#include "balinx_parser.hpp"
#include <iostream> 

int main(int argc, char* argv[]) {
    std::string filename = "build.bx";
    if(argc > 1 ){
        filename = argv[1];
    }

    BalinxParser balin(filename);

    if(balin.parse()) {
        //Do some epic parsing
        std::cout << "-----------------------------------------------------\n";
        std::cout << "Version: ";
        for(int i = 0; i < balin.get_version_number().size(); ++i) {
            std:: cout << balin.get_version_number()[i];
            if(i < balin.get_version_number().size() - 1) {
                std::cout << ".";
            }
        }
        std::cout << "\nExecutable Name: ";
        std::cout << balin.get_executable_name() << "\n";
        std::cout << "C_Compiler: " << balin.get_c_compiler();
        std::cout << "\nCPP_Compiler: " << balin.get_cpp_compiler();


        return 0;
    }
    else {
        return 1;
    }
    
    
    
}
