#include "../../lib/catch2/catch2.hpp"

#include "../custom_generators.hpp"
#include "../../core/cell_t.hpp"

#define TAGS "[core][cell_t]"

namespace Picross
{
    SCENARIO("Cell value validity is properly checked for", TAGS)
    {
        GIVEN("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            THEN("Values are considered valid")
            {
                REQUIRE(isValidCellValue(values));
            }
        }

        AND_GIVEN("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);

            THEN("Values are considered invalid")
            {
                REQUIRE_FALSE(isValidCellValue(values));
            }

            AND_THEN("Validity function throws when asked to")
            {
                REQUIRE_THROWS_AS(isValidCellValue(values, true), std::runtime_error);
            }
        }
    }

    SCENARIO("Cell values can all be turned to strings", TAGS)
    {
        GIVEN("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            THEN("Valid strings are returned")
            {
                using Catch::Matchers::StartsWith;
                REQUIRE_THAT(cellValueToString(values), StartsWith("CELL_"));
            }
        }

        AND_GIVEN("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);

            THEN("An exception is thrown")
            {
                REQUIRE_THROWS_AS(cellValueToString(values), std::runtime_error);
            }
        }
    }
}