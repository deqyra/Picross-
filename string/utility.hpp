#ifndef STRING_UTILITY_HPP
#define STRING_UTILITY_HPP

#include <string>

namespace StringUtil
{
    // Removes any single trailing '\r' at the end of a string.
    void popCR(std::string& str);

    // Checks whether a string contains a certain substring a certain amount of times.
    bool stringContains(const std::string& str, const char sub, int n = 1);

    // Checks whether a string is only made of digits.
    bool stringIsNum(const std::string& str);
}

#endif//STRING_UTILITY_HPP