#include "../../lib/catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include "../../cli/cli_state.hpp"

#define TAGS "[cli][cli_state]"

namespace Picross
{
    SCENARIO("CLI state works", TAGS)
    {
        GIVEN("Valid arguments")
        {
            CLIState state = CLIState(std::cin, std::cout, std::cerr);
            THEN("Constructor works")
            {
                REQUIRE_NOTHROW(CLIState(std::cin, std::cout, std::cerr));
            }

            AND_THEN("Getters work")
            {
                // Encapsulated streams write to and read from the same buffers as those provided
                REQUIRE(state.in().rdbuf() == std::cin.rdbuf());
                REQUIRE(state.out().rdbuf() == std::cout.rdbuf());
                REQUIRE(state.err().rdbuf() == std::cerr.rdbuf());
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