#include "../../lib/catch2/catch2.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#include "../../core/cell_t.hpp"
#include "../../core/grid.hpp"
#include "../../core/exceptions/invalid_grid_hints_error.hpp"
#include "../../core/exceptions/invalid_cell_value_error.hpp"
#include "../../tools/exceptions/index_out_of_bounds_error.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[core][grid]"

namespace Picross
{
    TEST_CASE("Grid constructor", TAGS)
    {
        SECTION("Size constructor")
        {
            Grid g = Grid(0, 0);
            REQUIRE(g.getWidth() == 0);
            REQUIRE(g.getHeight() == 0);

            g = Grid(5, 10);
            REQUIRE(g.getWidth() == 5);
            REQUIRE(g.getHeight() == 10);
        }

        SECTION("Size and hints constructor")
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

            SECTION("Hints are correct for grid dimensions")
            {
                REQUIRE_NOTHROW(Grid(5, 5, hHints, vHints));
            }

            SECTION("Hint entries are too many for grid dimensions")
            {
                REQUIRE_THROWS_AS(Grid(3, 3, hHints, vHints), InvalidGridHintsError);
            }

            SECTION("Hint entries are too few for grid dimensions")
            {
                REQUIRE_THROWS_AS(Grid(7, 7, hHints, vHints), InvalidGridHintsError);
            }

            SECTION("Hints do not fit in grid dimensions")
            {
                // Make hints the same size as the grid to be created.
                hHints.pop_back();
                hHints.pop_back();
                vHints.pop_back();
                vHints.pop_back();

                REQUIRE_THROWS_AS(Grid(3, 3, hHints, vHints), InvalidGridHintsError);
            }
        }
    }

    TEST_CASE("Grid cell/row/column getters and setters", TAGS)
    {
        Grid g = Grid(5, 5);
        SECTION("Dimension getters")
        {
            REQUIRE(g.getWidth() == 5);
            REQUIRE(g.getHeight() == 5);
        }

        SECTION("Default state is cleared")
        {
            for (int i = 0; i < g.getHeight(); i++)
            {
                for (int j = 0; j < g.getWidth(); j++)
                {
                    REQUIRE(g.getCell(i, j) == CELL_CLEARED);
                }
            }
        }

        SECTION("Getters and setters are coherent")
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

        SECTION("Getters and setters throw when asked impossible stuff")
        {
            REQUIRE_THROWS_AS(g.getCell(10, 0), IndexOutOfBoundsError);
            REQUIRE_THROWS_AS(g.getCell(0, 10), IndexOutOfBoundsError);
            REQUIRE_THROWS_AS(g.setCell(10, 0, CELL_CHECKED), IndexOutOfBoundsError);
            REQUIRE_THROWS_AS(g.setCell(0, 10, CELL_CHECKED), IndexOutOfBoundsError);
            REQUIRE_THROWS_AS(g.setCell(0, 0, CELL_T_VALUE_COUNT), InvalidCellValueError);
        }

        SECTION("Range setter")
        {
            REQUIRE_NOTHROW(g.setCellRange(2, 4, 1, 3, CELL_CHECKED));
            for (int i = 2; i <= 4; i++)
            {
                for (int j = 1; j <= 3; j++)
                {
                    REQUIRE(g.getCell(i, j) == CELL_CHECKED);
                }
            }
        }
    }

    TEST_CASE("Grid hint getters and setters")
    {
        Grid g = Grid(5, 5);

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

        SECTION("Getters and setters are coherent")
        {
            // Setting all row hints at once
            REQUIRE_NOTHROW(g.setAllRowHints(hHints));
            REQUIRE(g.getAllRowHints() == hHints);

            // Setting hints for one row
            hHints[2].push_back(1);
            REQUIRE_NOTHROW(g.setRowHints(2, {1}));
            REQUIRE(g.getAllRowHints() == hHints);

            // Single row hint getter
            std::vector<int> expected = {1};
            REQUIRE(g.getRowHints(2) == expected);

            // All the same but for columns
            REQUIRE_NOTHROW(g.setAllColHints(vHints));
            REQUIRE(g.getAllColHints() == vHints);

            vHints[2].push_back(1);
            REQUIRE_NOTHROW(g.setColHints(2, {1}));
            REQUIRE(g.getAllColHints() == vHints);

            REQUIRE_THROWS_AS(g.setColHints(10, {}), IndexOutOfBoundsError);
            REQUIRE_THROWS_AS(g.setColHints(2, {2, 2, 2}), InvalidGridHintsError);

            REQUIRE(g.getColHints(2) == expected);

        }

        SECTION("Getters and setters throw when asked impossible stuff")
        {
            // Throws when coordinate is off limits
            REQUIRE_THROWS_AS(g.setRowHints(10, {}), IndexOutOfBoundsError);
            // Throws when hint required space is above grid dimension
            REQUIRE_THROWS_AS(g.setRowHints(2, {2, 2, 2}), InvalidGridHintsError);
        }
    }

    TEST_CASE("Grid coordinate valididty check")
    {
        Grid g = Grid(5, 5);

        REQUIRE_NOTHROW(g.isValidRow(4, true));
        REQUIRE_NOTHROW(g.isValidCol(4, true));
        REQUIRE_NOTHROW(g.isValidCell(3, 4, true));

        REQUIRE_THROWS_AS(g.isValidRow(5, true), IndexOutOfBoundsError);
        REQUIRE_THROWS_AS(g.isValidCol(5, true), IndexOutOfBoundsError);
        REQUIRE_THROWS_AS(g.isValidCell(5, 5, true), IndexOutOfBoundsError);
    }

    TEST_CASE("Grid hints fit check")
    {
        Grid g = Grid(5, 5);

        REQUIRE_NOTHROW(g.areValidColHints({3, 1}, true));
        REQUIRE_THROWS_AS(g.areValidColHints({3, 3}, true), InvalidGridHintsError);

        REQUIRE_NOTHROW(g.areValidRowHints({3, 1}, true));
        REQUIRE_THROWS_AS(g.areValidRowHints({3, 3}, true), InvalidGridHintsError);
    }

    TEST_CASE("Grid hint consistency check")
    {
        Grid g = Grid(5, 5);

        XMLGridSerialzer xmlReader = XMLGridSerialzer();
        Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

        REQUIRE(reference.hintsAreConsistent());

        reference.setColHints(4, {5});
        REQUIRE_FALSE(reference.hintsAreConsistent());

        reference.setRowHints(0, {1, 1, 2});
        reference.setRowHints(1, {1, 1, 2});
        reference.setRowHints(2, {1, 1});
        // Such a grid is not solvable but its hints are consistent.
        REQUIRE(reference.hintsAreConsistent());
    }

    TEST_CASE("Grid hints generation", TAGS)
    {
        XMLGridSerialzer xmlReader = XMLGridSerialzer();
        Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");
        Grid test = reference;
        std::vector<std::vector<int>> emptyHints = std::vector<std::vector<int>>(10, std::vector<int>());
        
        // Remove all hints from one grid and see if re-generating them works.
        test.setAllColHints(emptyHints);
        test.setAllRowHints(emptyHints);
        REQUIRE(test != reference);

        REQUIRE_NOTHROW(test.setHintsFromState());
        REQUIRE(test == reference);
    }

    TEST_CASE("Grid solved check", TAGS)
    {
        XMLGridSerialzer xmlReader = XMLGridSerialzer();
        Grid reference = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

        REQUIRE(reference.isSolved());

        reference.checkCell(3, 5);
        REQUIRE_FALSE(reference.isSolved());
        reference.clearCell(3, 5);

        reference.setColHints(4, {5});
        REQUIRE_FALSE(reference.isSolved());
    }

    TEST_CASE("Grid most present state check", TAGS)
    {
        Grid g = Grid(5, 5);
        REQUIRE(g.mostPresentState() == CELL_CLEARED);

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                g.checkCell(i, j);
            }
        }
        REQUIRE(g.mostPresentState() == CELL_CHECKED);
    }

    TEST_CASE("Grid comparison", TAGS)
    {
        XMLGridSerialzer xmlReader = XMLGridSerialzer();
        Grid grid1 = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");
        Grid grid2 = xmlReader.loadGridFromFile("resources/tests/core/10_10_partial.xml");

        SECTION("Equality")
        {
            REQUIRE(grid1 == grid2);

            grid2.checkCell(3,5);
            REQUIRE_FALSE(grid1 == grid2);
        }

        SECTION("Inequality")
        {
            REQUIRE_FALSE(grid1 != grid2);

            grid2.checkCell(3,5);
            REQUIRE(grid1 != grid2);
        }
    }
}