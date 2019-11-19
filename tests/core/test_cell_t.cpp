#include "../../lib/catch2/catch2.hpp"

#include "../custom_generators.hpp"
#include "../../core/cell_t.hpp"
#include "../../core/exceptions/invalid_cell_value_error.hpp"

#define TAGS "[core][cell_t]"

namespace Picross
{
    TEST_CASE("Cell value validity", TAGS)
    {
        SECTION("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));
            REQUIRE(isValidCellValue(values));
        }

        SECTION("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);
            REQUIRE_FALSE(isValidCellValue(values));
            REQUIRE_THROWS_AS(isValidCellValue(values, true), InvalidCellValueError);
        }
    }

    TEST_CASE("Cell value naming", TAGS)
    {
        SECTION("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));

            using Catch::Matchers::StartsWith;
            REQUIRE_THAT(cellValueName(values), StartsWith("CELL_"));
        }

        SECTION("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);
            REQUIRE_THROWS_AS(cellValueName(values), InvalidCellValueError);
        }
    }

    TEST_CASE("Cell value int conversion", TAGS)
    {
        SECTION("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));
            REQUIRE_NOTHROW(cellValueToInt(values));
        }

        SECTION("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);
            REQUIRE_NOTHROW(cellValueToInt(values));
        }
    }

    TEST_CASE("Cell value string conversion", TAGS)
    {
        SECTION("Valid values")
        {
            auto values = GENERATE(array(CELL_T_ORDERED_VALUES, CELL_T_VALUE_COUNT));
            REQUIRE_NOTHROW(cellValueToString(values));
        }

        SECTION("Invalid values")
        {
            auto values = GENERATE(-1, CELL_T_VALUE_COUNT);
            REQUIRE_NOTHROW(cellValueToString(values));
        }
    }
}