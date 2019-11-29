#include "../../lib/catch2/catch2.hpp"

#include "../../io/text_grid_formatter.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../core/grid.hpp"
#include "../../tools/string_tools.hpp"

#define TAGS "[io][text][grid][formatting]"

namespace Picross
{
    TEST_CASE("Grid text render without hints", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/io/10_10_partial.xml");
        std::string expected = StringTools::readFileIntoString("resources/tests/io/10_10_partial_formatted_no_hints.txt");

        TextGridFormatter txt = TextGridFormatter();
        REQUIRE(txt.renderGrid(g) == expected);
    }

    TEST_CASE("Grid text render with hints", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/io/10_10_partial.xml");
        std::string expected = StringTools::readFileIntoString("resources/tests/io/10_10_partial_formatted_with_hints.txt");

        TextGridFormatter txt = TextGridFormatter();
        REQUIRE(txt.renderGridWithHints(g) == expected);
    }

    TEST_CASE("Grid almost without hints text render with hints")
    {
        Grid g = Grid(10, 10);
        g.setColHints(1, {1});
        g.setRowHints(1, {1});

        TextGridFormatter txt = TextGridFormatter();
        std::string expected = StringTools::readFileIntoString("resources/tests/io/10_10_almost_no_hints_formatted_with_hints.txt");
        REQUIRE(txt.renderGridWithHints(g) == expected);
    }

    TEST_CASE("Grid without hints text render with hints")
    {
        Grid g = Grid(10, 10);
        TextGridFormatter txt = TextGridFormatter();
        REQUIRE(txt.renderGridWithHints(g) == txt.renderGrid(g));
    }

    TEST_CASE("Complete grid with hints taking 2 digits")
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/io/20_20_solved.xml");
        std::string expected = StringTools::readFileIntoString("resources/tests/io/20_20_solved_formatted.txt");

        TextGridFormatter txt = TextGridFormatter();
        REQUIRE(txt.renderGridWithHints(g) == expected);
    }
}