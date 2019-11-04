#include "../../lib/catch2/catch2.hpp"

#include "../custom_generators.hpp"
#include "../../core/cell_t.hpp"
#include "../../core/exceptions/invalid_cell_value_error.hpp"

#define TAGS "[core][cell_t]"

namespace Picross
{
    SCENARIO("Cell value validity is properly checked for and valid valid can be named", TAGS)
    {
        GIVEN("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            THEN("Values are considered valid")
            {
                REQUIRE(isValidCellValue(values));
            }

            AND_THEN("Values can be named")
            {
                using Catch::Matchers::StartsWith;
                REQUIRE_THAT(cellValueName(values), StartsWith("CELL_"));
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
                REQUIRE_THROWS_AS(isValidCellValue(values, true), InvalidCellValueError);
            }

            AND_THEN("Trying to name them throws")
            {
                REQUIRE_THROWS_AS(cellValueName(values), InvalidCellValueError);
            }
        }
    }

    SCENARIO("Any cell value can be turned to an int and a string", TAGS)
    {
        GIVEN("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            THEN("Valid ints and strings are returned")
            {
                REQUIRE_NOTHROW(cellValueToInt(values));
                REQUIRE_NOTHROW(cellValueToString(values));
            }
        }

        AND_GIVEN("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);

            THEN("Valid ints and strings are returned")
            {
                REQUIRE_NOTHROW(cellValueToInt(values));
                REQUIRE_NOTHROW(cellValueToString(values));
            }
        }
    }
}