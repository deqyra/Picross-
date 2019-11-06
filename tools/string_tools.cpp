#include "string_tools.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../tools/exceptions/file_not_found_error.hpp"

namespace std
{
    std::string to_string(std::string val)
    {
        return val;
    }
}

namespace StringTools
{
    bool popChar(std::string& str, const char c)
    {
        if (str.size() == 0) return false;

        if (str.back() == c)
        {
            str.pop_back();
            return true;
        }
        return false;
    }

    bool popString(std::string& str, const std::string& sub)
    {
        if (str == sub)
        {
            str = "";
            return true;
        }

        if (str.size() < sub.size()) return false;

        // Check whether substring is contained at the endFill of the main string.
        if (str.substr(str.size() - sub.size(), sub.size()) == sub)
        {
            str = str.substr(0, str.size() - sub.size());
            return true;
        }
        return false;
    }

    bool popCR(std::string& str)
    {
        return popChar(str, '\r');
    }

    bool popLF(std::string& str)
    {
        return popChar(str, '\n');
    }

    bool popCRLF(std::string& str)
    {
        return popString(str, "\r\n");
    }

    bool stringContains(const std::string& str, const char sub, int n, bool exact)
    {
        if (str.size() == 0) return n == 0;
        if (!exact && n == 0) return true;

        // Counter for found occurrences.
        int i = 0;
        // Kind of a string cursor.
        std::size_t found = -1;

        // Search the string while the cursor is valid
        do
        {
            // Search only after the lastly found position
            found = str.find(sub, found + 1);

            if (found != std::string::npos)
            {
                i++;
                if (!exact && i == n)
                {
                    return true;
                }
                if (exact && i > n)
                {
                    return false;
                }
            }
        } while (found != std::string::npos);

        if (i == n)
        {
            return true;
        }

        return false;
    }

    bool stringIsNum(const std::string& str)
    {
        if (str.size() == 0) return false;
        
        for (auto it = str.begin(); it != str.end(); it++)
        {
            // Return false at the first non-digit char found.
            if (!std::isdigit(*it)) return false;
        }
        return true;
    }

    std::vector<std::string> tokenizeString(const std::string& str, char delimiter, bool discardEmptyTokens)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);

        // Repeatedly extract tokens from the input string.
        while (std::getline(tokenStream, token, delimiter))
        {
            // If specified so, empty tokens should not be added to the output vector.
            if (discardEmptyTokens && token == "") continue;
            
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string readFileIntoString(const std::string& path, bool stripCarriageReturns)
    {
        std::ifstream f = std::ifstream(path.c_str(), std::ios::in);
        if (f)
        {
            std::stringstream s;
            s << f.rdbuf();
            f.close();

            std::string res = s.str();
            if (stripCarriageReturns)
            {
                stripCR(res);
            }

            return res;
        }

        throw FileNotFoundError(path + ": file not found.");
    }

    void stripCR(std::string& str)
    {
        std::string result;
        std::vector<std::string> tokens = tokenizeString(str, '\r');
        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            //popCR(*it);
            result += *it;
        }
        str = result;        
    }
}
