#include "../../lib/catch2/catch2.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#include "../template_scenario.hpp"
#include "../../core/cell_t.hpp"
#include "../../core/grid.hpp"
#include "../../core/utility.hpp"
#include "../../tools/string_tools.hpp"
#include "../../tools/iterable_tools.hpp"

#define TAGS "[core][utility]"

namespace Picross
{
    SCENARIO("Minimum grid space required can be computed from hint sequence", TAGS)
    {
        GIVEN("A hint sequence")
        {
            std::vector<int> seq = {5, 2, 1};

            THEN("The correct minimum space is returned")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 10);
            }
        }

        AND_GIVEN("Another hint sequence")
        {
            std::vector<int> seq = {10, 32, 63};

            THEN("The correct minimum space is returned")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 107);
            }
        }

        AND_GIVEN("A hint sequence with negative hints")
        {
            std::vector<int> seq = {4, -6, 10};

            THEN("A minimum space is returned even though it doesn't make sense semantically")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 10);
            }
        }
    }

    SCENARIO("Hint generation and hint satisfaction check work properly", TAGS)
    {
        GIVEN("Different layouts of cells")
        {
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

            THEN("The check works properly")
            {
                REQUIRE(cellsSatisfyHints(layout1, hints1));
                REQUIRE(cellsSatisfyHints(layout2, hints1));
                REQUIRE_FALSE(cellsSatisfyHints(layout3, hints1));
            }

            AND_THEN("Hint generation works properly")
            {
                REQUIRE(hintsFromCells(layout1) == hints1);
                REQUIRE(hintsFromCells(layout2) == hints1);
                REQUIRE(hintsFromCells(layout3) == hints3);
            }
        }
    }
}