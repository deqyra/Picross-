#ifndef STRING__UTILITY_HPP
#define STRING__UTILITY_HPP

#include <string>
#include <vector>

namespace StringUtil
{
    // Removes any given single trailing character at the end of a string. Returns whether operation was successful.
    bool popChar(std::string& str, const char c);

    // Removes any given trailing substring at the end of a string. Returns whether operation was successful.
    bool popString(std::string& str, const std::string& sub);

    // Removes any single trailing '\r' at the end of a string. Returns whether operation was successful.
    bool popCR(std::string& str);

    // Removes any single trailing '\n' at the end of a string. Returns whether operation was successful.
    bool popLF(std::string& str);

    // Removes any trailing '\r\n' at the end of a string. Returns whether operation was successful.
    bool popCRLF(std::string& str);

    // Checks whether a string contains a certain substring a certain amount of times.
    bool stringContains(const std::string& str, const char sub, int n = 1, bool exact = false);

    // Checks whether a string is only made of digits.
    bool stringIsNum(const std::string& str);

    // Splits a string into substring based on a given delimiter, and returns all substrings in a vector.
    std::vector<std::string> tokenizeString(const std::string& str, char delimiter, bool discardEmptyTokens = false);

    // Open the file located at the provided path and returns a string filled with all of its contents.
    std::string readFileIntoString(const std::string& path, bool stripCarriageReturns = true);

    // Remove carriage returns in provided string. Returns whether operation was successful.
    void stripCR(std::string& str);
}

#endif//STRING__UTILITY_HPP