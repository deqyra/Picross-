#include "../../lib/catch2/catch2.hpp"

#include "../generate_static_grids.hpp"
#include "../../io/text_grid_formatter.hpp"
#include "../../core/grid.hpp"
#include "../../tools/string_tools.hpp"

#define TAGS "[io][text][grid][formatting]"

namespace Picross
{
    TEST_CASE("Text grid render no hints", TAGS)
    {
        TextGridFormatter txt = TextGridFormatter();
        Grid g = generate10x10PartialGrid(true);
        std::string expected = StringTools::readFileIntoString("resources/tests/io/10_10_partial_formatted_no_hints.txt");
        REQUIRE(txt.renderGrid(g) == expected);
    }

    TEST_CASE("Text grid render with hints", TAGS)
    {
        TextGridFormatter txt = TextGridFormatter();
        Grid g = generate10x10PartialGrid(true);
        std::string expected = StringTools::readFileIntoString("resources/tests/io/10_10_partial_formatted_with_hints.txt");
        REQUIRE(txt.renderGridWithHints(g) == expected);
    }
}