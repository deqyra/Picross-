#include "utility.hpp"

#include <string>
#include <vector>
#include <sstream>

#include "cell_t.hpp"
#include "grid.hpp"
#include "../string/utility.hpp"

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
        std::vector<std::string> tokens = StringUtil::tokenizeString(str, delimiter, true);

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

            if (!StringUtil::stringIsNum(*it))
            {
                throw std::invalid_argument("Provided string does not represent an integer.");
            }

            // Put parsed tokens in output vector.
            hints.push_back(std::stoi(*it));
        }

        return hints;
    }

	std::string multilineConcatenation(std::string first, std::string second)
	{
        // Step 1: split input strings into vectors of lines.
		std::vector<std::string> firstTokens = StringUtil::tokenizeString(first, '\n');
		std::vector<std::string> secondTokens = StringUtil::tokenizeString(second, '\n');
        
        // Step 2: remove potential trailing '\r' from every line in both vectors (can happen on Windows).
        for (auto it = firstTokens.begin(); it != firstTokens.end(); it++)
        {
            StringUtil::popCR(*it);
        }
        
        for (auto it = secondTokens.begin(); it != secondTokens.end(); it++)
        {
            StringUtil::popCR(*it);
        }

        // Step 3: iterate on both vectors at the same time and join their lines together.
        auto itFirst = firstTokens.begin();
        auto itSecond = secondTokens.begin();

        // Loop control: stop when either vector reaches its end.
        bool stop = itFirst == firstTokens.end() || itSecond == secondTokens.end();

        std::string s;
        while (!stop)
        {
            // Step 3.1: feed first line into stream.
            if (itFirst != firstTokens.end())
            {
                s += *itFirst;
                itFirst++;
            }

            // Step 3.2: feed second line into stream.
            if (itSecond != secondTokens.end())
            {
                s += *itSecond;
                itSecond++;
            }

            // Don't forget to advance iterators each time a line is accessed.

            // Step 3.3: break line and check for loop end.
            s += '\n';
            stop = itFirst == firstTokens.end() && itSecond == secondTokens.end();
        }

        // Step 4: return single string with everything.
        return s;
	}

    bool cellsSatisfyHints(std::vector<cell_t> cells, std::vector<int> hints)
    {
        // Cells satisfy hints if the hints generated from them are the same as those provided.
        return hintsFromCells(cells) == hints;
    }

    std::vector<int> hintsFromCells(std::vector<cell_t> cells)
    {
        std::vector<int> hints;
        int count = 0;

        for (auto it = cells.begin(); it != cells.end(); it++)
        {
            // If the cell is checked, count it.
            if (*it == CELL_CHECKED)
            {
                count++;
                continue;
            }

            // Otherwise, current sequence is over: register count if non-0 and reset count variable.
            if (count)
            {
                hints.push_back(count);
                count = 0;
            }
        }

        // Make sure to register the last count if non-0.
        if (count)
        {
            hints.push_back(count);
        }

        return hints;
    }
}