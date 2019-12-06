#include "iterative_solver.hpp"

#include <string>
#include <vector>

#include "../core/grid.hpp"
#include "../core/cell_t.hpp"

namespace Picross
{
    IterativeSolver::~IterativeSolver()
    {

    }

    std::string IterativeSolver::name()
    {
        return "Iterative solver";
    }

    void IterativeSolver::solve(Grid& grid)
    {
        Grid lastState = grid;

        // Whether the grid is solved.
        bool solved = false;
        // Whether the solving is stalled (no changes after a full iteration).
        bool stalled = false;

        while (!solved && !stalled)
        {
            // Iterate.
            for (int i = 0; i < grid.getHeight(); i++)
            {
                std::vector<cell_t> result = solveArray(grid.getRow(i), grid.getRowHints(i));
                grid.setRow(i, result);
            }

            for (int j = 0; j < grid.getWidth(); j++)
            {
                std::vector<cell_t> result = solveArray(grid.getCol(j), grid.getRowHints(j));
                grid.setCol(j, result);
            }

            // Update loop control.
            solved = grid.isSolved();
            stalled = (grid == lastState);
            // Register last state.
            lastState = grid;
        }
    }

    std::vector<cell_t> solveArray(std::vector<cell_t> array, std::vector<int> hints)
    {
        
    }
}
