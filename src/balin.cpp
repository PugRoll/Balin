#include "../include/balin.hpp"
#include "../include/balinx_parser.hpp"
#include "../include/balin_common.hpp"
#include "../include/cache.hpp"
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

        ParsedData *cacheData;


        std::ostringstream cmdStream;
        cmdStream << cpp_compiler << " ";

        //Let's substitue the variables
        processFile();
        checkDependencies();

        //Iterate through vector
        for (const std::string& include : includes) {
            cmdStream << "-I " << substituteVars(include, parser.get_variables());
        }

        for (const std::string& minecrafters: debugs) {
            cmdStream << " -g " << substituteVars(minecrafters, parser.get_variables());
        }


        for (const std::string& dep : deps) {
            cmdStream << " ./build/" << dep << ".o";
        }
        cmdStream << " -" << flags[1] << " " << executable;

        std::string command = cmdStream.str();

        std::cout << "\nCommand to be executed:\n" << command << "\n";

        int result = std::system(command.c_str());

        //lock for whether I want to compile or not
        if(finalCheck()) { /* TODO: replace "false" with "finalcheck()" */
            if(result != 0) {
                std::cerr << "Command failed to execute\n";
                return false;
            }
            else {
                std::cout << "Command executed successfully\n";
                //Lets cache if we were successful
                createCacheFile();
                writeToCacheFile("cc", hash(parser.get_c_compiler().c_str()));
                writeToCacheFile("cppc", hash(parser.get_cpp_compiler().c_str()));

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
            deps = parser.get_deps();
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


bool Balin::checkDependencies() {
    //We should check for the object file in the build directory, if it doesn't exist
    //we should get if available
    for(std::string dep : deps) {
        if(checkAgainstDependencyList(dep)) {
            return true;
        }
    }

    
   return false;
}



bool Balin::checkAgainstDependencyList(const std::string dep) {
    std::ifstream file = std::ifstream("/home/Patrick/.balin/balinDeps.txt");
    std::string line;
    std::string curr;

    unsigned int hashLine;
    unsigned int hashDep = hash(dep.c_str());

    std::vector<std::string> tokens;

    const char delimeter = ':';

    while(std::getline(file, line)) {
        if(line.empty()) {
            std::cerr << "Dependency was not found\n";
            return false;
        }

        std::istringstream iss(line);
        std::string str;
        while(getline(iss, str, delimeter)) {
            tokens.push_back(str);        
            /* Name of token is located at tokens[0]
             * Location of token is located at tokens[1] */
        }

        //TODO: ADD proper functionality to this
        //TODO: Check for valid dependency
        //Copy the object file from the directory
        curlAndUnzip(tokens[0].c_str());
    }


    return false;
}


void Balin::curlAndUnzip(const char* depName) {
    std::ostringstream cmd;
    const char* serverName = "http://localhost:8080/downloads/";

    //Command to copy the file
    cmd << "curl --url " << serverName << depName << ".tar.gz" << " --output ./build/" << depName << ".tar.gz";
    std::system(cmd.str().c_str());
    //std::cout << cmd.str().c_str() << "\n";

    //unzip the tar.gz file
    cmd.str("");
    cmd << "tar -xf ./build/" << depName << ".tar.gz" << " -C ./build/";
    std::system(cmd.str().c_str());
    //std::cout << cmd.str().c_str() << "\n";

}

