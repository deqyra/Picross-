#include "../../lib/catch2/catch2.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#include "../../core/cell_t.hpp"
#include "../../core/grid.hpp"
#include "../../core/exceptions/invalid_grid_hints_error.hpp"
#include "../../core/exceptions/invalid_cell_value_error.hpp"
#include "../../core/exceptions/out_of_bounds_grid_coordinates_error.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[core][grid]"

namespace Picross
{
    SCENARIO("Grid constructors work", TAGS)
    {
        Grid g = Grid(0, 0);
    
        GIVEN("Any arguments")
        {
            THEN("The size constructor works")
            {
                g = Grid(0, 0);
                REQUIRE(g.getWidth() == 0);
                REQUIRE(g.getHeight() == 0);

                g = Grid(5, 10);
                REQUIRE(g.getWidth() == 5);
                REQUIRE(g.getHeight() == 10);
            }
        }

        std::vector<std::vector<int>> hHints;
        hHints.push_back({2, 2});
        hHints.push_back({1, 1});
        hHints.push_back({});
        hHints.push_back({1, 1});
        hHints.push_back({2, 2});

        std::vector<std::vector<int>> vHints;
        vHints.push_back({2, 2});
        vHints.push_back({1, 1});
        vHints.push_back({});
        vHints.push_back({1, 1});
        vHints.push_back({2, 2});

        AND_GIVEN("Valid arguments")
        {
            THEN("Hint constructor works")
            {
                REQUIRE_NOTHROW(Grid(5, 5, hHints, vHints));
            }
        }

        AND_GIVEN("Invalid arguments")
        {
            THEN("Hint constructor throws when given hints requiring more space than grid dimension")
            {
                REQUIRE_THROWS_AS(Grid(3, 3, hHints, vHints), InvalidGridHintsError);
            }
        }
    }

    SCENARIO("Grid getters/setters/modifiers work", TAGS)
    {
        Grid g = Grid(5, 5);
        GIVEN("A grid")
        {
            THEN("Dimension getters work")
            {
                REQUIRE(g.getWidth() == 5);
                REQUIRE(g.getHeight() == 5);
            }

            AND_THEN("Default state is all cleared")
            {
                for (int i = 0; i < g.getHeight(); i++)
                {
                    for (int j = 0; j < g.getWidth(); j++)
                    {
                        REQUIRE(g.getCell(i, j) == CELL_CLEARED);
                    }
                }
            }

            AND_THEN("Cell/row/column getters and setters work and are coherent with each other")
            {
                REQUIRE_NOTHROW(g.checkCell(1, 3));
                REQUIRE(g.getCell(1, 3) == CELL_CHECKED);

                REQUIRE_NOTHROW(g.crossCell(3, 4));
                REQUIRE(g.getCell(3, 4) == CELL_CROSSED);

                REQUIRE_NOTHROW(g.clearCell(3, 4));
                REQUIRE(g.getCell(3, 4) == CELL_CLEARED);

                REQUIRE_NOTHROW(g.setCell(1, 3, CELL_CLEARED));
                REQUIRE(g.getCell(1, 3) == CELL_CLEARED);

                REQUIRE_NOTHROW(g.setCell(1, 3, CELL_CROSSED));
                REQUIRE(g.getCell(1, 3) == CELL_CROSSED);

                REQUIRE_NOTHROW(g.setCell(3, 4, CELL_CHECKED));
                REQUIRE(g.getCell(3, 4) == CELL_CHECKED);

                std::vector<cell_t> expectedRow = {
                    CELL_CLEARED,
                    CELL_CLEARED,
                    CELL_CLEARED,
                    CELL_CLEARED,
                    CELL_CHECKED
                };

                std::vector<cell_t> expectedCol = {
                    CELL_CLEARED,
                    CELL_CROSSED,
                    CELL_CLEARED,
                    CELL_CLEARED,
                    CELL_CLEARED
                };

                REQUIRE(g.getRow(3) == expectedRow);
                REQUIRE(g.getCol(3) == expectedCol);
            }

            AND_THEN("Cell/row/column getters and setters throw when asked impossible coordinates or non-existent cell value")
            {
                REQUIRE_THROWS_AS(g.getCell(10, 0), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.getCell(0, 10), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.setCell(10, 0, CELL_CHECKED), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.setCell(0, 10, CELL_CHECKED), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.setCell(0, 0, CELL_T_VALUE_COUNT), InvalidCellValueError);
            }

            AND_THEN("Hint setters work")
            {
                std::vector<std::vector<int>> hHints;
                hHints.push_back({2, 2});
                hHints.push_back({1, 1});
                hHints.push_back({});
                hHints.push_back({1, 1});
                hHints.push_back({2, 2});

                std::vector<std::vector<int>> vHints;
                vHints.push_back({2, 2});
                vHints.push_back({1, 1});
                vHints.push_back({});
                vHints.push_back({1, 1});
                vHints.push_back({2, 2});

                // Setting all row hints at once
                REQUIRE_NOTHROW(g.setAllRowHints(hHints));
                REQUIRE(g.getAllRowHints() == hHints);

                // Setting hints for one row
                hHints[2].push_back(1);
                REQUIRE_NOTHROW(g.setRowHints(2, {1}));
                REQUIRE(g.getAllRowHints() == hHints);

                // Throws when coordinate is off limits
                REQUIRE_THROWS_AS(g.setRowHints(10, {}), OutOfBoundsGridCoordinatesError);
                // Throws when hint required space is above grid dimension
                REQUIRE_THROWS_AS(g.setRowHints(2, {2, 2, 2}), InvalidGridHintsError);

                // Single row hint getter
                std::vector<int> expected = {1};
                REQUIRE(g.getRowHints(2) == expected);

                // Same but for columns
                REQUIRE_NOTHROW(g.setAllColHints(vHints));
                REQUIRE(g.getAllColHints() == vHints);

                vHints[2].push_back(1);
                REQUIRE_NOTHROW(g.setColHints(2, {1}));
                REQUIRE(g.getAllColHints() == vHints);

                REQUIRE_THROWS_AS(g.setColHints(10, {}), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.setColHints(2, {2, 2, 2}), InvalidGridHintsError);

                REQUIRE(g.getColHints(2) == expected);
            }
        }
    }

    SCENARIO("Checks work", TAGS)
    {
        Grid g = Grid(5, 5);

        GIVEN("Various arguments")
        {
            THEN("Checks behave properly")
            {
                REQUIRE_NOTHROW(g.isValidRow(4, true));
                REQUIRE_NOTHROW(g.isValidCol(4, true));
                REQUIRE_NOTHROW(g.isValidCell(3, 4, true));

                REQUIRE_THROWS_AS(g.isValidRow(5, true), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.isValidCol(5, true), OutOfBoundsGridCoordinatesError);
                REQUIRE_THROWS_AS(g.isValidCell(5, 5, true), OutOfBoundsGridCoordinatesError);

                REQUIRE_NOTHROW(g.areValidColHints({3, 1}, true));
                REQUIRE_THROWS_AS(g.areValidColHints({3, 3}, true), InvalidGridHintsError);

                REQUIRE_NOTHROW(g.areValidRowHints({3, 1}, true));
                REQUIRE_THROWS_AS(g.areValidRowHints({3, 3}, true), InvalidGridHintsError);
            }
        }
    }

    SCENARIO("Setting hints from state works", TAGS)
    {
        GIVEN("Some grid")
        {
            XMLGridSerialzer xmlReader = XMLGridSerialzer();
            Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");
            Grid test = reference;
            std::vector<std::vector<int>> emptyHints = std::vector<std::vector<int>>(10, std::vector<int>());
            // Remove all hints from one grid and see if re-generating them works.
            test.setAllColHints(emptyHints);
            test.setAllRowHints(emptyHints);
            REQUIRE(test != reference);

            THEN("Hints are properly generated")
            {
                REQUIRE_NOTHROW(test.setHintsFromState());
                REQUIRE(test == reference);
            }
        }
    }

    SCENARIO("Hint consistency check works", TAGS)
    {
        GIVEN("Some grid")
        {
            XMLGridSerialzer xmlReader = XMLGridSerialzer();
            Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

            THEN("Hint consistency check works")
            {
                REQUIRE(reference.hintsAreConsistent());

                reference.setColHints(4, {5});
                REQUIRE_FALSE(reference.hintsAreConsistent());

                reference.setRowHints(0, {1, 1, 2});
                reference.setRowHints(1, {1, 1, 2});
                reference.setRowHints(2, {1, 1});
                // Such a grid is not solvable but its hints are consistent.
                REQUIRE(reference.hintsAreConsistent());
            }
        }
    }

    SCENARIO("Solved check works", TAGS)
    {
        GIVEN("Some grid")
        {
            XMLGridSerialzer xmlReader = XMLGridSerialzer();
            Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

            THEN("Solved check works")
            {
                REQUIRE(reference.isSolved());

                reference.checkCell(3, 5);
                REQUIRE_FALSE(reference.isSolved());
                reference.clearCell(3, 5);

                reference.setColHints(4, {5});
                REQUIRE_FALSE(reference.isSolved());
            }
        }
    }

    SCENARIO("Grid most present state is coherent", TAGS)
    {
        Grid g = Grid(5, 5);

        GIVEN("An empty grid")
        {
            THEN("Most present state is cleared")
            {
                REQUIRE(g.mostPresentState() == CELL_CLEARED);
            }
        }

        GIVEN("A half-checked grid")
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    g.checkCell(i, j);
                }
            }

            THEN("Most present state is checked")
            {
                REQUIRE(g.mostPresentState() == CELL_CHECKED);
            }
        }
    }

    SCENARIO("Grid comparison works", TAGS)
    {
        GIVEN("A couple grids")
        {
            XMLGridSerialzer xmlReader = XMLGridSerialzer();
            Grid grid1 = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");
            Grid grid2 = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

            THEN("== works")
            {
                REQUIRE(grid1 == grid2);

                grid2.checkCell(3,5);
                REQUIRE_FALSE(grid1 == grid2);
            }

            AND_THEN("!= works")
            {
                REQUIRE_FALSE(grid1 != grid2);

                grid2.checkCell(3,5);
                REQUIRE(grid1 != grid2);
            }
        }
    }
}