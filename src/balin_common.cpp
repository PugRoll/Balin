#include "../include/balin_common.hpp"

size_t split(const std::string &line, std::vector<std::string> &str, const char delim) {
    size_t pos = line.find(delim);
    size_t initial_pos = 0;

    str.clear();

    while(pos != std::string::npos) {
        str.push_back(line.substr(initial_pos, pos - initial_pos));
        initial_pos = pos + 1;

        pos = line.find(delim, initial_pos);
    }


    str.push_back(line.substr(initial_pos, std::min(pos, str.size()) - initial_pos + 1));

    return str.size();
}

