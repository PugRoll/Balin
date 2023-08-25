#ifndef BALINX_PARSER_H
#define BALINX_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

class BalinxParser {
public:
    explicit BalinxParser(const std::string& filename);
    bool parse();

    const std::string& get_executable_name() const {return executable_name_; }
    const std::vector<int>& get_version_number() const {return version_number_; }
    const std::string& get_filename() const  {return filename_; }
    const std::string& get_c_compiler() const {return c_compiler; }
    const std::string& get_cpp_compiler() const {return cpp_compiler; }
    const std::vector<std::pair<std::string, std::string>>& get_variables() const {return variables; }
private: 
    bool handle_version(const std::string& line);
    bool handle_executable(const std::string& line);
    bool handle_c_compiler(const std::string& line);
    bool handle_cpp_compiler(const std::string& line);
    bool addVariable(const std::string& line);
    void printMessage(const std::string& line);

    std::ifstream file_;
    std::string executable_name_;
    std::vector<int> version_number_;
    std::string filename_;
    std::string c_compiler;
    std::string cpp_compiler;
    std::vector<std::pair<std::string, std::string>> variables;




};
//Function to parse the build file


#endif // BALINX_PARSER_H
