#include "../include/balin.hpp"
#include "../include/balinx_parser.hpp"
#include "../include/balin_common.hpp"
#include "../include/cache.hpp"
#include "../include/balin-curl.hpp"
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

        ParsedData cacheData = currentCacheData();

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

        for (const std::string& lib : libs) {
            cmdStream << " -l" << substituteVars(lib, parser.get_variables());
        }

        std::string command = cmdStream.str();

        std::ostringstream minecraft;
        minecraft << "Executing command: " << command;
        balinDebug(minecraft);


        int result = std::system(command.c_str());

        //lock for whether I want to compile or not
        if(finalCheck()) { /* TODO: replace "false" with "finalcheck()" */
            if(result != 0) {
                balinError("Command failed to execute");
                return false;
            }
            else {
                balinInfo("Command executed successfully");
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
            libs = parser.get_libs();
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
                std::ostringstream str;
                str << "[Found]: " << variablePlaceholder;
                balinInfo(str);

                str.clear();

                result.replace(pos, variablePlaceholder.length(), pair.second);
                pos = result.find(variablePlaceholder);
                str << "[Replacing with]: " << pair.second;
                balinInfo(str);
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
        std::string str;
        str = "No compiler specified for: " + lang;
        balinError(str);
        return false; //In the case a compiler is not specified
    }

    std::ostringstream cmd;

    cmd << compiler << " --version";

    std::string gamerTime = cmd.str();
    gamerTime = gamerTime + " > nul";
    int result = std::system(gamerTime.c_str());
    std::system("rm nul");
    if(result != 0) {
        std::string str;
        str =  compiler + " failed to validate compiler";
        balinError(str);
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

    if(checkWithCache(cpp_compiler) || checkWithCache(c_compiler)) {
        return true;
    }

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
       balinInfo("Valid path");
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
        cacheFile << "#11/14 Make a cache file\n";
        cacheFile.close();
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
            balinError("Dependency was not found");
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
        //tokens[0] is the name of the dependency
        unsigned int id = id_getWithName(tokens[0]);

        if(id == BALIN_CURL_ERROR) {
            std::ostringstream str;
            str << tokens[0] << "could not be found"; 
            balinError(str);
            return false;
        }

        downloadArchiveFromDB(id, tokens[0]);
        unzipArchive(tokens[0].c_str());

    }


    return false;
}


void Balin::unzipArchive(const char* depName) {
    std::ostringstream cmd;
    cmd << "tar -xzvf ./build/" << depName << ".tar.gz " << "-C ./build/";
    std::system(cmd.str().c_str());
}


bool Balin::checkWithCache(const std::string curr) {
    return ((hash(curr.c_str()) == (getValueWithIdentifier(curr))
        ));
}

