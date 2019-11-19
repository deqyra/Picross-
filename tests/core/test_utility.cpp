#include "../../lib/catch2/catch2.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#include "../../core/cell_t.hpp"
#include "../../core/grid.hpp"
#include "../../core/utility.hpp"
#include "../../tools/string_tools.hpp"
#include "../../tools/iterable_tools.hpp"

#define TAGS "[core][utility]"

namespace Picross
{
    TEST_CASE("Compute minimum space from hints", TAGS)
    {
        std::vector<int> seq = {5, 2, 1};
        REQUIRE(minimumSpaceFromHints(seq) == 10);

        seq = {10, 32, 63};
        REQUIRE(minimumSpaceFromHints(seq) == 107);

        seq = {4, -6, 10};
        REQUIRE(minimumSpaceFromHints(seq) == 10);
    }

    std::vector<cell_t> layout1 = {
        CELL_CLEARED,   // 
        CELL_CROSSED,   // X
        CELL_CLEARED,   // 
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CROSSED,   // X
        CELL_CLEARED,   // 
        CELL_CLEARED,   // 
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CLEARED,   // 
        CELL_CROSSED,   // X
        CELL_CROSSED,   // X
        CELL_CLEARED,   // 
        CELL_CROSSED,   // X
        CELL_CLEARED,   // 
        CELL_CHECKED,   // O
        CELL_CLEARED,   // X
        CELL_CLEARED    // X
    };

    std::vector<cell_t> layout2 = {
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CLEARED,   // 
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CROSSED,   // X
        CELL_CHECKED    // O
    };

    std::vector<cell_t> layout3 = {
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CHECKED,   // O
        CELL_CROSSED,   // X
        CELL_CHECKED    // O
    };

    std::vector<int> hints1 = {3, 4, 1};
    std::vector<int> hints3 = {5, 1};

    TEST_CASE("Check for whether cells satisfy hints", TAGS)
    {
        REQUIRE(cellsSatisfyHints(layout1, hints1));
        REQUIRE(cellsSatisfyHints(layout2, hints1));
        REQUIRE_FALSE(cellsSatisfyHints(layout3, hints1));
    }

    TEST_CASE("Hint generation from cells", TAGS)
    {
        REQUIRE(hintsFromCells(layout1) == hints1);
        REQUIRE(hintsFromCells(layout2) == hints1);
        REQUIRE(hintsFromCells(layout3) == hints3);
    }
}