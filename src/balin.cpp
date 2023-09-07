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

        //Let's substitue the variables
        processFile();

        //Iterate through vector
        for (const std::string& include : includes) {
            cmdStream << "-I " << substituteVars(include, parser.get_variables());
        }

        cmdStream << " -" << flags[1] << " " << executable;

        std::string command = cmdStream.str();

        std::cout << "\nCommand to be executed:\n" << command << "\n";

        int result = std::system(command.c_str());

        //lock for whether I want to compile or not
        if(true) {
            if(result != 0) {
                std::cerr << "Command failed to execute\n";
                return false;
            }
            else {
                std::cout << "Command executed successfully\n";
                return true;
            }
        }
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



std::string Balin::substituteVars(const std::string input, const std::vector<std::pair<std::string, std::string>>& vars) {
        std::string result = input;
        for (const auto& pair : vars) {
            std::string variablePlaceholder = "$" + pair.first;
            size_t pos = result.find(variablePlaceholder);
            while(pos != std::string::npos) {
                std::cout << "\t[Found]: " << variablePlaceholder << "\n";
                result.replace(pos, variablePlaceholder.length(), pair.second);
                pos = result.find(variablePlaceholder);
                std:: cout << "\t[Replacing with]: " << pair.second << "\n";
            }
        }
        return result;
}


void Balin::processFile() {
    std::ifstream file = std::ifstream(Balin::get_filename());
    //We can assume that the file can be opened because it was successfully parsed already
    std::string line;
    while(std::getline(file, line)) {
        if(line.empty() || line.front() == '#') {
            continue;
        }

        std::istringstream iss(line);
        Balin::substituteVars(line, parser.get_variables());
    }

    
}




