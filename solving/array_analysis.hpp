#ifndef SOLVING__ARRAY_ANALYSIS_HPP
#define SOLVING__ARRAY_ANALYSIS_HPP

#include <vector>

#include "../core/cell_t.hpp"
#include "../tools/array_2d.hpp"

namespace Picross
{
namespace ArrayAnalaysis
{
    // Tries to match checked input cells with input hints. Returns a 2D array of bool of dimensions (cells, hints), telling whether a given cell may belong to a given hint.
    Array2D<bool> matchCellsToHints(const std::vector<cell_t>& array, const std::vector<int>& hints);
    // Given an input array and a cell index, find the closest crossed cell left of index.
    int closestCrossLeft(const std::vector<cell_t>& array, int index);
    // Given an input array and a cell index, find the closest crossed cell right of index.
    int closestCrossRight(const std::vector<cell_t>& array, int index);
    // Given an input array and a cell index, find the boundaries of the group of checked cells containing that index.
    void findGroupBoundaries(const std::vector<cell_t>& array, int index, int& leftBoundIndex, int& rightBoundINdex);
    // Given a 2D array indicating cell-hint matches, returns the list of cell indices that were matched to only one hint.
    std::vector<int> findIndicesOfSingleHintCells(const Array2D<bool>& matchEntries);
    // Some processing to detect and remove false positives from match entries.
    void removeFalsePositiveMatches(Array2D<bool>& matchEntries);
} // namespace ArrayAnalaysis
} // namespace Picross

#endif//SOLVING__ARRAY_ANALYSIS_HPP