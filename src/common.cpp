#include <iostream>

constexpr unsigned int hash(const char* str)  {
    unsigned int hashValue = 0;
    while(*str) {
        hashValue = (hashValue << 5) + *str++;
    }

    return hashValue;
}
