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
        doBefore();
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

        for (const std::string& debug: debugs) {
            cmdStream << " -g " << substituteVars(debug, parser.get_variables());
        }


        for (const std::string& dep : deps) {
            balinDevDebug("Dependency: " + dep);
            cmdStream << " ./build/" << dep << ".o";
        }
        cmdStream << " -" << flags[1] << " " << executable;

        for (const std::string& lib : libs) {
            cmdStream << " -l" << substituteVars(lib, parser.get_variables());
        }

        std::string command = cmdStream.str();

        std::ostringstream ostr;
        ostr << "Executing command: " << command;
        balinDebug(ostr);


        int result = std::system(command.c_str());

        //lock for whether I want to compile or not
        if(finalCheck()) { /* TODO: replace "false" with "finalcheck()" */
            if(result != 0) {
                balinError("Command failed to execute");
                return false;
            }
            else {
                balinDebug("Command executed successfully");
                //Lets cache if we were successful
                createCacheFile();
                writeToCacheFile("cc", hash(parser.get_c_compiler().c_str()));
                writeToCacheFile("cppc", hash(parser.get_cpp_compiler().c_str()));
                doAfter();
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
                balinDevDebug(str);

                str.clear();

                result.replace(pos, variablePlaceholder.length(), pair.second);
                pos = result.find(variablePlaceholder);
                str << "[Replacing with]: " << pair.second;
                balinDevDebug(str);
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

    std::string cmdStr = cmd.str();
    cmdStr = cmdStr + " > nul";
    int result = std::system(cmdStr.c_str());
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
       balinDevDebug("Valid path");
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
    unsigned int id;
    for(std::string dep : deps) {
           id = id_getWithName(dep); 

           downloadArchiveFromDB(id, dep);
           unzipArchive(dep.c_str());
    }

    
   return false;
}

void Balin::unzipArchive(const char* depName) {
    std::ostringstream cmd;
    cmd << "tar -xzf ./build/" << depName << ".tar.gz " << "-C ./build/";
    std::system(cmd.str().c_str());
}


bool Balin::checkWithCache(const std::string curr) {
    return ((hash(curr.c_str()) == (getValueWithIdentifier(curr))
        ));
}

bool Balin::doBefore() {
    balinDevDebug("Called doBefore");
    std::vector<std::string> tasks = parser.get_before_tasks();
    std::string curr;
    const std::string BALIN_PROPERTY = "balin.property";
    for(const auto& t : tasks) { 
        size_t pos = t.find(BALIN_PROPERTY);
        if(pos != std::string::npos) {
            //We found a property
            size_t propPos = t.rfind('.');
            std::string sub = t.substr(propPos + 1);
            balinDevDebug(sub);
        }
        else {
            //Not a property
            //Going to assume its a command
            std::system(t.c_str());
        }
    }
    return true;
}

bool Balin::doAfter() {
    balinDevDebug("Called doAfter");
    std::vector<std::string> tasks = parser.get_after_tasks();
    std::string curr;
    const std::string BALIN_PROPERTY = "balin.property";
    for(const auto& t : tasks) { 
        size_t pos = t.find(BALIN_PROPERTY);
        if(pos != std::string::npos) {
            //We found a property
            size_t propPos = t.rfind('.');
            std::string sub = t.substr(propPos + 1);
            balinDevDebug(sub);
        }
        else {
            //Not a property
            //Going to assume its a command
            std::system(t.c_str());
        }
    }
    return true;
}
