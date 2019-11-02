#ifndef STRING_UTILITY_HPP
#define STRING_UTILITY_HPP

#include <string>

namespace StringUtil
{
    // Removes any given single trailing character at the end of a string.
    bool popChar(std::string& str, const char c);

    // Removes any given trailing substring at the end of a string.
    bool popString(std::string& str, const std::string& sub);

    // Removes any single trailing '\r' at the end of a string.
    bool popCR(std::string& str);

    // Removes any single trailing '\n' at the end of a string.
    bool popLF(std::string& str);

    // Removes any trailing '\r\n' at the end of a string.
    bool popCRLF(std::string& str);

    // Checks whether a string contains a certain substring a certain amount of times.
    bool stringContains(const std::string& str, const char sub, int n = 1, bool exact = false);

    // Checks whether a string is only made of digits.
    bool stringIsNum(const std::string& str);
}

#endif//STRING_UTILITY_HPP