#ifndef BALIN_COMMON_H 
#define BALIN_COMMON_H
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <fstream>

/**
 * Terminal colors
 */
namespace balin_color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[091m";
    const std::string OKGREEN = "\033[092m";
    const std::string YELLOW = "\033[093m";
    const std::string OKBLUE = "\033[094m";
}


std::fstream& balin_gotoLine(std::fstream& file, unsigned int num);



//size_t balin_split(const std::string line, std::vector<std::string> &str, const char delim);

/**
 * Functionalilty: return a simple hash value
 * @Param: char* str -> C style string to hash
 */
constexpr unsigned int hash(const char* str)  {
    unsigned int hashValue = 0;
    while(*str) {
        hashValue = (hashValue << 5) + *str++;
    }
    return hashValue;
}

/**
 * Small container for flags
 */
struct balin_flags {
    bool infoFlag;
    bool debugFlag;
    bool devFlag;
} extern bFlags;

/**
 * Functionality:
 * @Param: string line: take a line with quotes: "" and extract the string within
 */
std::string resolveString(const std::string& line);

/**
 * Functionality: Indicate success
 */
void balinSuccess();

/**
 * Functionality: indicate a Failure
 * @Param: char* str: reason for failure
 */
void balinFailure(const char* str);

/**
 *  Functionality: Print out  some string, beginning with "[INFO]:" tag
 *  @Param: char* str: string to print out
 */
void balinInfo(const char* str);

/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG]:" tag
 * @Param: char* str: string to print out
 */
void balinDebug(const char* str);


/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG]:" tag
 * Notes: For development use
 * @Param: char* str: string to print out
 */

void balinDevDebug(const char* str);

/**
 * Functionality: Print out some string, beginning with a red: "[ERROR]:" tag
 * @Param: char* error : string to print out
 */
void balinError(const char* error);

/**
 *  Functionality: Print out some string, beginning with "[INFO]:" tag
 *  Notes: Overloaded balinInfo function
 *  @Param: string str: string to print out
 */
void balinInfo(const std::string& str);

/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG:]" tag
 * Notes: Overloaded balinDebug function
 * @Param: string str: string to print out
 */
void balinDebug(const std::string& str);

/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG]:" tag
 * Notes: Overloaded balinDevDebug function
 * Notes: For development use
 * @Param: string str: string to print out
 */
void balinDevDebug(const std::string& str);

/**
 * Functionality: Print out some string, beginning with a red: "[ERROR]:" tag
 * Notes: Overloaded balinError function
 * @Param: string error: error to print out
 */

void balinError(const std::string& error);

/**
 *  Functionality: Print out some string, beginning with "[INFO]:"  tag
 *  Notes: Overloaded balinInfo function
 *  @Param: ostringstream str: string to print out
 */
void balinInfo(const std::ostringstream& str);

/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG]:" tag
 * Notes: Overloaded balinDebug function
 * @Param: ostringstream str: string to print out
 */
void balinDebug(const std::ostringstream& str);

/**
 * Functionality: Print out some string, beginning with a red: "[ERROR]:" tag
 * Notes: Overloaded balinError function
 * @Param: ostringstream error: error to print out
 */
void balinError(const std::ostringstream& error);

/**
 * Functionality: Print out some string, beginning with a yellow: "[DEBUG]:" tag
 * Notes: Overloaded balinDevDebug function
 * Notes: For development use
 * @Param: ostringstream str: string to print out
 */
void balinDevDebug(const std::ostringstream& str);


void setInfoFlag(bool value);
void setDebugFlag(bool value);
void setDevDebugFlag(bool value);

void initFlags();


#endif
