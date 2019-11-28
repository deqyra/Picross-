#include "../../lib/catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include "../../picross_cli/picross_cli_state.hpp"

#define TAGS "[picross_cli][cli_state]"

namespace Picross
{
    TEST_CASE("CLIState constructor", TAGS)
    {
        PicrossCLIState state = PicrossCLIState();
        // Default grid is constructed in state
        REQUIRE(state.grid() == Grid(0, 0));
    }

    TEST_CASE("CLIState getters and setters", TAGS)
    {
        PicrossCLIState state = PicrossCLIState();
        Grid g = Grid(5, 5);
        REQUIRE_NOTHROW(state.grid() = g);
        REQUIRE(state.grid() == g);
    }
}