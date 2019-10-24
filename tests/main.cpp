#define CATCH_CONFIG_MAIN
#include "../lib/catch2/catch2.hpp"

TEST_CASE("Test", "[test]")
{
    REQUIRE(1 == 1);
}