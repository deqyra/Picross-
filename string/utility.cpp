#include "utility.hpp"

#include <string>

namespace StringUtil
{
    void popCR(std::string& str)
    {
        if (str.size() == 0) return;

        if (str.back() == '\r')
        {
            str.pop_back();
        }
    }

    void popLF(std::string& str)
    {
        if (str.size() == 0) return;

        if (str.back() == '\n')
        {
            str.pop_back();
        }
    }

    void popCRLF(std::string& str)
    {
        if (str.size() < 2) return;

        if (str.substr(str.size() - 3, 2) == "\r\n")
        {
            str = str.substr(0, str.size() - 2);
        }
    }

    bool stringContains(const std::string& str, const char sub, int n)
    {
        // Counter for found occurrences.
        int i = 0;
        // Kind of a string cursor.
        std::size_t found = -1;

        // Search the string while the cursor is valid
        while (found != std::string::npos)
        {
            // Search only after the lastly found position
            str.find(sub, found + 1);

            if (found != std::string::npos)
            {
                i++;
                if (i == n)
                {
                    return true;
                }
            }
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
