#ifndef CORE__UTILITY_HPP
#define CORE__UTILITY_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "cell_t.hpp"
#include "grid.hpp"
#include "../tools/string_tools.hpp"

namespace Picross
{
    // Returns the minimum cell length needed for a hint sequence to be satisfied.
    int minimumSpaceFromHints(const std::vector<int>& hints);

    // Tells whether the layout of the provided vector of cells satisfies the provided hints.
    bool cellsSatisfyHints(const std::vector<cell_t>& cells, const std::vector<int>& hints);

    // Generates the vector of hints that is satisfied by the provided cells.
    std::vector<int> hintsFromCells(const std::vector<cell_t>& cells);

    // Returns the outcome of a merging attempt given a source value, a destination value and a merging policy.
    cell_t mergeSingleCell(cell_t destination, cell_t source, int mergingPolicy);
} // namespace Picross

#endif//CORE__UTILITY_HPP