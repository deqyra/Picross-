#include "utility.hpp"

#include <iostream>
#include <string>

namespace StringUtil
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
        if (str.size() < sub.size()) return false;

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
        for (auto it = str.begin(); it != str.end(); it++)
        {
            // Return false at the first non-digit char found.
            if (!std::isdigit(*it)) return false;
        }
        return true;
    }
}
