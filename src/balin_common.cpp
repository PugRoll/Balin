#include "../include/balin_common.hpp"



std::fstream& balin_gotoLine(std::fstream& file, unsigned int num) {
    file.seekg(std::ios::beg);
    for(int i = 0; i < num - 1; ++i) {
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return file;
}


balin_flags bFlags;


std::string resolveString(const std::string& line) {
    size_t last = line.find_last_of("\"");
    size_t first = line.find("\"");
    std::string msg = "";
    if(first != std::string::npos && last != std::string::npos && last > first) {
        msg = line.substr(first + 1, last - first - 1);
        return msg;
        
    }
    else {
        balinError("Unresolved string error");
    }
    return "";
}



void initFlags() {
    bFlags.infoFlag = true;
    bFlags.debugFlag = false;
}

void balinInfo(const char* str) {
    if(bFlags.infoFlag) {
        std::cout << "\t[INFO]: " << str << "\r\n";
    }
}

void balinDebug(const char* str) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW <<"\t[DEBUG]: " << balin_color::RESET <<str << "\r\n";
    }
}

void balinDevDebug(const char* str) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW <<"\t[DEBUG]: " << balin_color::RESET <<str << "\r\n";
    }
}

void balinError(const char* error) {
    std::cerr << balin_color::RED << "\t[ERROR]: " << balin_color::RESET << error << "\r\n";
}

void balinInfo(const std::string& str) {
    if(bFlags.infoFlag) {
        std::cout << "\t[INFO]: " << str << "\r\n";
    }
}

void balinDebug(const std::string& str) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW << "\t[DEBUG]: " << balin_color::RESET << str << "\r\n";
    }
}

void balinDevDebug(const std::string& str) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW << "\t[DEBUG]: " << balin_color::RESET << str << "\r\n";
    }
}
void balinError(const std::string& error) {
    std::cerr << balin_color::RED << "\t[ERROR]: " << balin_color::RESET << error << "\r\n";
}

void balinInfo(const std::ostringstream& oss) {
    if(bFlags.infoFlag) {
        std::cout << "\t[INFO]: " << oss.str() << "\r\n";
    }
}

void balinDebug(const std::ostringstream& oss) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW << "\t[DEBUG]: " << balin_color::RESET << oss.str() << "\r\n";
    }
}

void balinDevDebug(const std::ostringstream& oss) {
    if(bFlags.debugFlag) {
        std::cout << balin_color::YELLOW << "\t[DEBUG]: " << balin_color::RESET << oss.str() << "\r\n";
    }
}

void balinError(const std::ostringstream& oss_error) {
    std::cerr << balin_color::RED << "\t[ERROR]: " << balin_color::RESET << oss_error.str() << "\r\n";
}


void setInfoFlag(bool value) {
    bFlags.infoFlag = value;
}
void setDebugFlag(bool value) {
    bFlags.debugFlag = value;
}

void setDevDebugFlag(bool value) {
    bFlags.devFlag = value;
}

void balinSuccess() {
    if(bFlags.infoFlag) {
        std::cout << balin_color::OKGREEN << "\t[INFO]: Success\r\n" << balin_color::RESET;
    }
}

void balinFailure(const char* str) {
    std::cout << balin_color::RED << "\t[ERROR]: Failure with: " << balin_color::RESET << str << "\r\n";
}



//size_t balin_split(const std::string &line, std::vector<std::string> &str, const char delim) {
//    size_t pos = line.find(delim);
//    size_t initial_pos = 0;
//
//    str.clear();
//
//    while(pos != std::string::npos) {
//        str.push_back(line.substr(initial_pos, pos - initial_pos));
//        initial_pos = pos + 1;
//
//        pos = line.find(delim, initial_pos);
//    }
//
//
//    str.push_back(line.substr(initial_pos, std::min(pos, str.size()) - initial_pos + 1));

//    return str.size();
//}
