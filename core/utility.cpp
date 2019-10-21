#include "utility.hpp"

#include <sstream>

static void removeCarriageReturnAtEnd(std::string &str)
{
    if (str.back() == '\r')
    {
        str.pop_back();
    }
}

namespace Picross
{
    int minimumSpaceFromHints(const std::vector<int> &hints)
	{
		int space = 0;
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			space += (*it) + 1;
		}
		return space - 1;
	}

    std::vector<int> stringToHintVector(const std::string& str, char delimiter)
    {
        std::vector<int> hints;
        std::vector<std::string> tokens = tokenizeString(str, delimiter);

        for (auto it = tokens.begin(); it != tokens.end(); it++)
        {
            hints.push_back(std::stoi(*it));
        }

        return hints;
    }

    std::vector<std::string> tokenizeString(const std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

	std::string multilineConcatenation(std::string first, std::string second)
	{
		std::vector<std::string> firstTokens = tokenizeString(first, '\n');
		std::vector<std::string> secondTokens = tokenizeString(second, '\n');
        
        for (auto it = firstTokens.begin(); it != firstTokens.end(); it++)
        {
            removeCarriageReturnAtEnd(*it);
        }
        
        for (auto it = secondTokens.begin(); it != secondTokens.end(); it++)
        {
            removeCarriageReturnAtEnd(*it);
        }

        auto itFirst = firstTokens.begin();
        auto itSecond = secondTokens.begin();
        bool stop = itFirst != firstTokens.end() && itSecond != secondTokens.end();

        std::stringstream s;
        while (!stop)
        {
            if (itFirst != firstTokens.end())
            {
                s << *itFirst;
                itFirst++;
            }

            if (itSecond != secondTokens.end())
            {
                s << *itSecond;
                itSecond++;
            }

            s << std::endl;
            stop = itFirst != firstTokens.end() && itSecond != secondTokens.end();
        }

        return s.str();
	}

    cell_t mostPresentState(const Grid &grid)
    {
        int counts[CELL_T_VALUE_COUNT] = {0};

        for (int row = 0; row < grid.getHeight(); row++)
        {
            for (int col = 0; col < grid.getWidth(); col++)
            {
                counts[grid.getCell(row, col)]++;
            }
        }

        int maxIndex = indexOfMaxElement(counts, CELL_T_VALUE_COUNT);
        return CELL_T_ORDERED_VALUES[maxIndex];
    }
}