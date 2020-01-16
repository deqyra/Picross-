#include "array_analysis.hpp"

#include <vector>

#include "../core/cell_t.hpp"
#include "../tools/array_2d.hpp"

namespace Picross
{
namespace ArrayAnalysis
{
    Array2D<bool> matchCellsToHints(std::vector<cell_t> array, std::vector<int> hints)
    {
        Array2D<bool> belongEntries = Array2D<bool>(array.size(), hints.size(), false);


    }
} // namespace ArrayAnalysis
} // namespace Picross
