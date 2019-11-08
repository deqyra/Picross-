#include "../../lib/catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include "../../picross_cli/picross_cli_state.hpp"

#define TAGS "[picross_cli][cli_state]"

namespace Picross
{
    SCENARIO("CLI state works", TAGS)
    {
        GIVEN("Valid arguments")
        {
            PicrossCLIState state = PicrossCLIState();
            THEN("Constructor works")
            {
                REQUIRE_NOTHROW(PicrossCLIState());
            }

            AND_THEN("Getters work")
            {
                // Default grid is constructed in state
                REQUIRE(state.grid() == Grid(0, 0));
            }

            AND_THEN("Setters work")
            {
                Grid g = Grid(5, 5);
                REQUIRE_NOTHROW(state.grid() = g);
                REQUIRE(state.grid() == g);
            }
        }
    }
}