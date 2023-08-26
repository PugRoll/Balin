#include "../include/balinx_parser.hpp"
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
        std::cout << "\nVariables:\n"; 
        std::vector<std::pair<std::string, std::string>> vars = balin.get_variables();
        for(const auto& pair : vars) {
            std::cout << "\t"<< pair.first  << " -> " << pair.second << "\n";
        }
        std::cout << "\nIncludes: ";
        std::vector<std::string> str = balin.get_includes();
        for(const std::string& ite : str) {
            std::cout << "\t" << ite;
        }
        


        return 0;
    }
    else {
        return 1;
    }
    
    
    
}
