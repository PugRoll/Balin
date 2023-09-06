#include "../include/balin.hpp"
#include "../include/balinx_parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Balin::Balin(const std::string& filename) 
    : file_(filename), 
    filename_(filename), 
    parser(filename)
{}

bool Balin::compile() {
    if(setVars()) {
        std::ostringstream cmdStream;
        cmdStream << cpp_compiler << " ";

        //Iterate through vector
        for (const std::string& include : includes) {
            cmdStream << "-I" << include;
        }

        cmdStream << " -" << flags[1] << " " << executable;

        std::string command = cmdStream.str();

        std::cout << "\nCommand to be executed:\n" << command << "\n";
        return true;
    }
    return false;   
}


bool Balin::setVars() {
    if(parser.parse()) {
            executable = parser.get_executable_name();
            cpp_compiler = parser.get_cpp_compiler();
            includes = parser.get_includes();
            flags = parser.get_flags();
            return true;
    }
    return false;
}
