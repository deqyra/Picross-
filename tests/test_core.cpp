#include "../lib/catch2/catch2.hpp"

#include "custom_generators.hpp"
#include "../core/cell_t.hpp"

namespace Picross
{
    SCENARIO("Cell value validity is properly checked for", "[core][cell]")
    {
        GIVEN("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            WHEN("Validity is tested for")
            {
                THEN("Values are valid")
                {
                    REQUIRE(isValidCellValue(values));
                }
            }
        }

        AND_GIVEN("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);

            WHEN("Validity is tested for")
            {
                THEN("Values are invalid")
                {
                    REQUIRE_FALSE(isValidCellValue(values));
                }

                AND_THEN("Validity function throws when asked to")
                {
                    REQUIRE_THROWS_AS(isValidCellValue(values, true), std::runtime_error);
                }
            }
        }
    }
}