#ifndef TOOLS__STRING_TOOLS_HPP
#define TOOLS__STRING_TOOLS_HPP

#include <string>
#include <vector>

namespace std
{
    std::string to_string(std::string val);
}

namespace StringTools
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

    // Parses an int sequence from a string and returns it in a vector.
    std::vector<int> stringToIntVector(const std::string& str, char delimiter = ' ');

    // Open the file located at the provided path and returns a string filled with all of its contents.
    std::string readFileIntoString(const std::string& path, bool stripCarriageReturns = true);

    // Takes in two multi-line strings, concatenates them line by line and returns the result in one string.
    std::string multilineConcatenation(std::string first, std::string second);

    // Remove carriage returns in provided string. Returns whether operation was successful.
    void stripCR(std::string& str);

    // Parse two ints separated by a single character
    bool parseIntRange(std::string input, char delimiter, int& low, int& high);

    // Returns a string representation of the contents of an iterable.
    template<typename Iter>
    std::string iterableToString(
        Iter first, Iter last,          std::string delimiter = " ",
        std::string opening = "",       std::string closing = "",
        std::string elementPrefix = "", std::string elementSuffix = "")
    {
        std::string s = opening;

        for (auto it = first; it != last; it++)
        {
            s += elementPrefix + std::to_string(*it) + elementSuffix;
            if (it != last - 1)
            {
                s += delimiter;
            }
        }

        s += closing;

        // Output example:
        // vectorToString({5, 2, 1}, "-", "<", ">", "[", "]") == "<[5]-[2]-[1]>"

        return s;
    }

    template<typename Iterable>
    std::string iterableToString(
        Iterable container,             std::string delimiter = " ",
        std::string opening = "",       std::string closing = "",
        std::string elementPrefix = "", std::string elementSuffix = "")
    {
        return iterableToString(container.begin(), container.end(), delimiter, opening, closing, elementPrefix, elementSuffix);
    }
}

#endif//TOOLS__STRING_TOOLS_HPP