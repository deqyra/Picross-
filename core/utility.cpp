#include "utility.hpp"

#include <string>
#include <vector>
#include <sstream>

#include "cell_t.hpp"
#include "grid.hpp"
#include "exceptions/conflicting_merge_error.hpp"
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

        // One too many space is added at the last iteration of the loop, remove it (only if a non-zero space was calculated).
        if (space) space--;
		return space;
	}

    bool cellsSatisfyHints(const std::vector<cell_t>& cells, const std::vector<int>& hints)
    {
        // Cells satisfy hints if the hints generated from them are the same as those provided.
        return hintsFromCells(cells) == hints;
    }

    std::vector<int> hintsFromCells(const std::vector<cell_t>& cells)
    {
        // Count every sequence of checked cells and store the length of each in a vector.
        std::vector<int> hints;
        int count = 0;

        for (auto it = cells.begin(); it != cells.end(); it++)
        {
            // If the cell is checked, count it.
            if (*it == CELL_CHECKED)
            {
                count++;
            }
            // Otherwise, current sequence is over: register count if non-0 and reset count variable.
            else if (count)
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

    cell_t mergeSingleCell(cell_t destination, cell_t source, int mergingPolicy)
    {
        // If the source is cleared, keep whatever is in destination.
        if (source == CELL_CLEARED) return destination;
        // If the destination is cleared, put whatever is in source.
        if (destination == CELL_CLEARED) return source;
        // If both are equal, keep that value.
        if (destination == source) return destination;

        // Now there are only two cases left:
        // - source is checked, destination is crossed
        // - source is crossed, destination is checked

        // If the policy is to throw on any conflict, do so.
        switch (mergingPolicy)
        {
            case MERGING_POLICY_THROW_ON_CONFLICTS:
                throw ConflictingMergeError("Cannot merge checked and crossed cells.");
                break;
            case MERGING_POLICY_OVERWRITE:
                return source;
                break;
            case MERGING_POLICY_PRESERVE:
                return destination;
                break;
            default:
                throw std::invalid_argument("mergeSingleCell: unknown policy " + std::to_string(mergingPolicy));
        }
    }
} // namespace Picross
