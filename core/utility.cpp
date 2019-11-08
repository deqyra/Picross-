#include "utility.hpp"

#include <string>
#include <vector>
#include <sstream>

#include "cell_t.hpp"
#include "grid.hpp"
#include "../tools/string_tools.hpp"

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