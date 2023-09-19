#include "../include/balin.hpp"
#include "../include/balinx_parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

Balin::Balin(const std::string& filename) 
    : file_(filename), 
    filename_(filename), 
    parser(filename)
{}



bool Balin::compile() {
    if(setVars()) {
        //Go ahead and create the build directory
        createBuildDirectory();
        bool doCompile = false;
        std::ostringstream cmdStream;
        cmdStream << cpp_compiler << " ";

        //Let's substitue the variables
        processFile();

        //Iterate through vector
        for (const std::string& include : includes) {
            cmdStream << "-I " << substituteVars(include, parser.get_variables());
        }

        for (const std::string& minecrafters: debugs) {
            cmdStream << " -g " << substituteVars(minecrafters, parser.get_variables());
        }

        cmdStream << " -" << flags[1] << " " << executable;

        std::string command = cmdStream.str();

        std::cout << "\nCommand to be executed:\n" << command << "\n";

        int result = std::system(command.c_str());

        //lock for whether I want to compile or not
        if(finalCheck()) {
            if(result != 0) {
                std::cerr << "Command failed to execute\n";
                return false;
            }
            else {
                std::cout << "Command executed successfully\n";
                //Lets cache if we were successful
                createCacheFile();
                return true;
            }
        }
    }
    return false;   
}


bool Balin::setVars() {
    if(parser.parse()) {
            executable = parser.get_executable_name();

            c_compiler = parser.get_c_compiler();
            validCCompiler = (testCompiler(c_compiler, "c"));

            cpp_compiler = parser.get_cpp_compiler();
            Balin::validCppCompiler = (testCompiler(cpp_compiler, "c++"));

            includes = parser.get_includes();
            flags = parser.get_flags();
            debugs = parser.get_debugs();
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
        line = Balin::substituteVars(line, parser.get_variables());
    }    
}

bool Balin::testCompiler(const std::string& compiler, const std::string lang){
    if(compiler.empty()) {
        std::cerr << "\t[INFO] No compiler specified for: " << lang << "\n";
        return false; //In the case a compiler is not specified
    }

    std::ostringstream cmd;

    cmd << compiler << " --version";

    std::string gamerTime = cmd.str();
    gamerTime = gamerTime + " > nul";
    int result = std::system(gamerTime.c_str());
    std::system("rm nul");
    if(result != 0) {
        std::cerr << "\t[ERROR] " << compiler << " failed to validate compiler\n";
        return false;
    }
    else {
        return true;
    }
}

bool Balin::finalCheck() {
    //Check to make sure they are empty strings
    bool c_isNotEmpty = !c_compiler.empty();
    bool cpp_isNotEmpty = !cpp_compiler.empty();

    if(c_isNotEmpty && cpp_isNotEmpty) {
        return true;
    }

    //More specific checks
    if(!(c_isNotEmpty && cpp_isNotEmpty)) { //Both string values are empty
        return false;
    }
    else {
        if(!c_isNotEmpty && cpp_isNotEmpty && validCppCompiler) {
            return true;
        }
        if(c_isNotEmpty && validCCompiler && !cpp_isNotEmpty) {
            return false;
        }
    }

    return false;

}



void Balin::createBuildDirectory() {
   const char* dir = "./build"; 
   if(checkBuildDirectory()) {
       std::cout << "Valid path\n";
   }
   else {
       //If the directory does not exists let's create the directory
       std::ostringstream buildCmd;
       buildCmd << "mkdir " << dir;
       std::system(buildCmd.str().c_str()); //convert to string then to c_str()
   }
}

void Balin::createCacheFile() {
    //TODO: Logic to hash and check if there is any changes between the last change and the previous
    const char* cachePath = "./build/cache.bx";
    if(checkForCacheFile()) {
        //Cache file exists 
    }
    else { //Cache file does not exist or was deleted
        Balin::cacheFile.open(cachePath);
        cacheFile << "Make a cache file\n";
    }
    
}

bool Balin::checkBuildDirectory() {
    struct stat sb;
    return (stat("./build", &sb) == 0);
}

bool Balin::checkForCacheFile() {
    struct stat sb;
    return (stat("./build/cache.bx", &sb) == 0 && !(sb.st_mode &S_IFDIR));
}







