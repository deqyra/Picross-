#include "../../lib/catch2/catch2.hpp"

#include "../../io/xml_grid_serializer.hpp"
#include "../../core/grid.hpp"

#define TAGS "[io][xml][grid][serialization]"

static Picross::Grid generateExpectedGrid()
{
    Picross::Grid grid = Picross::Grid(10, 10);

    grid.setRowHints(0, {1, 1});
    grid.setRowHints(1, {1, 1});
    grid.setRowHints(2, {1});
    grid.setRowHints(3, {1});
    grid.setRowHints(4, {1});
    grid.setRowHints(5, {1});
    grid.setRowHints(6, {1, 1});
    grid.setRowHints(7, {1, 1});
    grid.setRowHints(8, {1});
    grid.setRowHints(9, {1, 1});

    grid.setColHints(0, {3, 1, 2, 1});
    grid.setColHints(2, {2, 1, 5});

    grid.checkCell(0, 0);
    grid.checkCell(1, 0);
    grid.checkCell(2, 0);
    grid.checkCell(4, 0);
    grid.checkCell(6, 0);
    grid.checkCell(7, 0);
    grid.checkCell(9, 0);
    grid.checkCell(0, 2);
    grid.checkCell(1, 2);
    grid.checkCell(3, 2);
    grid.checkCell(5, 2);
    grid.checkCell(6, 2);
    grid.checkCell(9, 2);
    grid.checkCell(7, 2);
    grid.checkCell(8, 2);

    return grid;
}

namespace Picross
{
    TEST_CASE("XMLGridSerializer properly deserializes valid XML files", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g(0, 0);
        REQUIRE_NOTHROW(g = xml.loadGridFromFile("resources/tests/io/10_10_partial.xml"));
        REQUIRE(g == generateExpectedGrid());
    }

    TEST_CASE("XMLGridSerializer properly serializes grids", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        REQUIRE_NOTHROW(xml.saveGridToFile(generateExpectedGrid(), "resources/tests/io/output.xml"));

        // If the grid was properly saved, the loaded grid should be the same.
        // (assuming deserialization works, which was made sure of by the above test case)
        Grid g = xml.loadGridFromFile("resources/tests/io/output.xml");
        REQUIRE(g == generateExpectedGrid());
    }
}
