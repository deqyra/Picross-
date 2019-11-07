#include "../../lib/catch2/catch2.hpp"

#include <iostream>
#include <fstream>
#include "../../tools/cli/cli_streams.hpp"

#define TAGS "[cli][cli_streams]"

SCENARIO("CLI stream wrapper works", TAGS)
{
    GIVEN("Valid arguments")
    {
        CLIStreams streams = CLIStreams(std::cin, std::cout, std::cerr);
        THEN("Constructor works")
        {
            REQUIRE_NOTHROW(CLIStreams(std::cin, std::cout, std::cerr));
        }

        AND_THEN("Getters work")
        {
            // Encapsulated streams write to and read from the same buffers as those provided
            REQUIRE(streams.in().rdbuf() == std::cin.rdbuf());
            REQUIRE(streams.out().rdbuf() == std::cout.rdbuf());
            REQUIRE(streams.err().rdbuf() == std::cerr.rdbuf());
        }
    }
}
