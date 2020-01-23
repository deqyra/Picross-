#include "array_analysis.hpp"

#include <algorithm>
#include <vector>
#include <functional>

#include "../core/cell_t.hpp"
#include "../core/utility.hpp"
#include "../tools/array_2d.hpp"

namespace Picross
{
namespace ArrayAnalysis
{
    Array2D<bool> matchCellsToHints(const std::vector<cell_t>& array, const std::vector<int>& hints)
    {
        Array2D<bool> matchEntries = Array2D<bool>(array.size(), hints.size(), false);

        for (auto it = array.begin(); it != array.end(); it++)
        {
            // Retrieve cell index.
            int i = it - array.begin();

            // Skip non checked cells, we are not trying to pair them with hints.
            if (*it != CELL_CHECKED) continue;

            // Find index of two closest crosses (if any).
            int leftCross = closestCrossLeft(array, i);
            int rightCross = closestCrossRight(array, i);

            for (auto nt = hints.begin(); nt != hints.end(); nt++)
            {
                // Retrieve hint index.
                int n = nt - hints.begin();

                // Compute minimum space required both left and right for other hints.
                int spaceRequiredLeft = minimumSpaceFromHints(std::vector(hints.begin(), nt));
                if (spaceRequiredLeft) spaceRequiredLeft++;

                int spaceRequiredRight = minimumSpaceFromHints(std::vector(nt + 1, hints.end()));
                if (spaceRequiredRight) spaceRequiredRight++;

                // Compute the max index which left and right groups can span over.
                int maxLeftIndex = spaceRequiredLeft;
                int maxRightIndex = array.size() - spaceRequiredRight - 1;

                // Restrict those to potentially closer crossed cells.
                // Add/draw 1 from the cross coordinate to get the cell *next* to that cross.
                maxLeftIndex = std::max(maxLeftIndex, leftCross + 1);
                maxRightIndex = std::min(maxRightIndex, rightCross - 1);

                // Check whether cell i is within allowed coordinates for clue n
                if ((i < maxLeftIndex) || (i > maxRightIndex)) continue;

                int leftBoundIndex;
                int rightBoundINdex;
                findGroupBoundaries(array, i, leftBoundIndex, rightBoundINdex);

                // Retrieve hint value and group size.
                int hintVal = *nt;
                int groupSize = (rightBoundINdex - leftBoundIndex) + 1;

                // Ignore if group is too big for hint.
                if (groupSize > hintVal) continue;

                // Ignore if group boundaries overlap max space.
                if (leftBoundIndex < maxLeftIndex) continue;
                if (rightBoundINdex > maxRightIndex) continue;

                // Ignore if max space cannot fit hint value.
                if ((maxRightIndex - maxLeftIndex) + 1 < hintVal) continue;

                // If control reaches here, all conditions for belonging have passed.
                // The current cell may belong to the current clue;
                matchEntries.setCell(i, n, true);
            }
        }
    }

    int closestCrossLeft(const std::vector<cell_t>& array, int index)
    {
        auto rEnd = array.rend();
        auto begin = rEnd - index;
        auto it = std::find(begin, rEnd, CELL_CROSSED);

        return (rEnd - it) - 1;

        // Illustrative example:
        // =====================
        //
        // Input index:        6
        // Cross index:        3
        // Input array:        ║   |   |   | × |   |   | i |   |   |   ║
        // Indices:            ║ 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 ║
        // Iterators:        ↑               ↑       ↑
        //                 rEnd             it     begin = rEnd-index
        //
        // rEnd-it = 4
        // (rEnd-it) - 1 = 3 = index to return
        //
        // If no cross is found, it == end, so (end-it) - 1 = -1, which is expected in such a case.
    }

    int closestCrossRight(const std::vector<cell_t>& array, int index)
    {
        auto end = array.end();
        auto begin = array.begin() + index + 1;
        auto it = std::find(begin, end, CELL_CROSSED);

        return it - begin;
        
        // If no cross is found, it == end, so (it-begin) = array.size(), which is expected in such a case.
    }

    void findGroupBoundaries(const std::vector<cell_t>& array, int index, int& leftBoundIndex, int& rightBoundINdex)
    {
        std::function<bool(const cell_t&)> isNonChecked = [] (const cell_t& val) {
            return val != CELL_CHECKED;
        };

        // Find the left boundary.
        auto rEnd = array.rend();
        auto rBegin = rEnd - index;
        auto itLeft = std::find_if(rBegin, rEnd, isNonChecked);
        // Same reasoning as for closestCrossLeft above,
        // but we're looking for the last previously matched cell.
        // The -1 shown above is cancelled by the +1 needed to get the previous index.
        leftBoundIndex = (rEnd - itLeft);

        // Find the right boundary.
        auto end = array.end();
        auto begin = array.begin() + index;
        auto itRight = std::find_if(begin, end, isNonChecked);
        rightBoundINdex = (itRight - begin) - 1;
    }

    std::vector<int> findIndicesOfSingleHintCells(const Array2D<bool>& matchEntries)
    {
        int nCells = matchEntries.getHeight();
        std::vector<int> result = std::vector<int>();

        // For each row (corresponding to a single cell)...
        for (int i = 0; i < nCells; i++)
        {
            // Retrieve the belonging entries for that cell.
            std::vector<bool> hintEntries = matchEntries.getRow(i);
            // Count how many hints it was matched to (value = true).
            int matchedHints = std::count(hintEntries.begin(), hintEntries.end(), true);
            
            // If exactly 1, add the cell index to result.
            if (matchedHints == 1) result.push_back(i);
        }

        return result;
    }

    void removeFalsePositiveMatches(Array2D<bool>& matchEntries)
    {
        std::vector<int> singleHintCells = findIndicesOfSingleHintCells(matchEntries);
    }
} // namespace ArrayAnalysis
} // namespace Picross
