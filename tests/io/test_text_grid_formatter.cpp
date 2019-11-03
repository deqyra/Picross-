#include "../../lib/catch2/catch2.hpp"

#include "generate_static_grids.hpp"
#include "../../io/text_grid_formatter.hpp"
#include "../../core/grid.hpp"
#include "../../string/utility.hpp"

#define TAGS "[io][xml][grid][serialization]"

namespace Picross
{
    TEST_CASE("Text grid formatter produces expected output")
    {
        TextGridFormatter txt = TextGridFormatter();
        Grid g = generate10x10PartialGrid();
        std::string expected = StringUtil::readFileIntoString("resources/tests/io/10_10_partial_formatted_no_hints.txt");
        REQUIRE(txt.renderGrid(g) == expected);

        g = generate10x10PartialGrid();
        expected = StringUtil::readFileIntoString("resources/tests/io/10_10_partial_formatted_with_hints.txt");
        REQUIRE(txt.renderGridWithHints(g) == expected);
    }
}