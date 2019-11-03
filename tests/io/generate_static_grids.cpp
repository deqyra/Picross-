#include "generate_static_grids.hpp"

#include "../../core/grid.hpp"

Picross::Grid generate10x10PartialGrid()
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
