#include "utility.hpp"

#include <string>
#include <vector>
#include <sstream>

#include "cell_t.hpp"
#include "grid.hpp"

// Removes any single trailing '\r' at the end of a string.
static void removeCarriageReturnAtEnd(std::string& str)
{
    if (str.back() == '\r')
    {
        str.pop_back();
    }
}

// Checks whether a string is only made of digits.
static bool stringIsNum(const std::string& str)
{
    for (auto it = str.begin(); it != str.end(); it++)
    {
        if (!std::isdigit(*it)) return false;
    }
    return true;
}

namespace Picross
{
    int minimumSpaceFromHints(const std::vector<int>& hints)
	{
		int space = 0;

        // The space required for a hint sequence to be satisfied is the sum of
        // all hints together with a single blank space inbetween each of them.
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			space += (*it) + 1;
		}
        // String example for sequence {5, 2, 3}:
        // "xxxxx xx xxx" <- length = (5 + 1) + (2 + 1) + 3 = 12

        // One too many space is added at the last iteration of the loop, remove it.
		return space - 1;
	}

    std::vector<int> stringToIntVector(const std::string& str, char delimiter)
    {
        std::vector<int> hints;
        // Tokenise input string according to provided delimiter (true = discard any empty token).
        std::vector<std::string> tokens = tokenizeString(str, delimiter, true);

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            // By default, std::stoi will successfully parse any string which begins with a digit, 
            // discarding the rest of the content from the point it cannot be parsed anymore.
            //
            // From the standpoint of this function, if string "53;<>!12aaa" was an extracted token,
            // then it is clearly invalid and should not be parsed.
            // However, std::stoi("53;<>!12aaa") does not throw and returns 53.
            //
            // This may be seen as kind of a behavioural discrepancy, leading to confusion on the
            // side of the user. Therefore, it is strictly required that any token must be 
            // exclusively comprised of digits, which is what the following condition ensures.
            //
            // This also prevents negative numbers from being accepted. Even though this does not
            // fall within the scope of that function stricto sensu, it fits its use cases.

            if (!stringIsNum(*it))
            {
                throw std::runtime_error("Provided string does not represent an integer.");
            }

            // Put parsed tokens in output vector.
            hints.push_back(std::stoi(*it));
        }

        return hints;
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

	std::string multilineConcatenation(std::string first, std::string second)
	{
        // Step 1: split input strings into vectors of lines.
		std::vector<std::string> firstTokens = tokenizeString(first, '\n');
		std::vector<std::string> secondTokens = tokenizeString(second, '\n');
        
        // Step 2: remove potential trailing '\r' from every line in both vectors (can happen on Windows).
        for (auto it = firstTokens.begin(); it != firstTokens.end(); it++)
        {
            removeCarriageReturnAtEnd(*it);
        }
        
        for (auto it = secondTokens.begin(); it != secondTokens.end(); it++)
        {
            removeCarriageReturnAtEnd(*it);
        }

        // Step 3: iterate on both vectors at the same time and join their lines together.
        auto itFirst = firstTokens.begin();
        auto itSecond = secondTokens.begin();

        // Loop control: stop when either vector reaches its end.
        bool stop = itFirst == firstTokens.end() || itSecond == secondTokens.end();

        std::stringstream s;
        while (!stop)
        {
            // Step 3.1: feed first line into stream.
            if (itFirst != firstTokens.end())
            {
                s << *itFirst;
                itFirst++;
            }

            // Step 3.2: feed second line into stream.
            if (itSecond != secondTokens.end())
            {
                s << *itSecond;
                itSecond++;
            }

            // Don't forget to advance iterators each time a line is accessed.

            // Step 3.3: break line and check for loop end.
            s << std::endl;
            stop = itFirst == firstTokens.end() && itSecond == secondTokens.end();
        }

        // Step 4: return single string with everything.
        return s.str();
	}
}