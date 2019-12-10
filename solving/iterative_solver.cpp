#include "iterative_solver.hpp"

#include <string>
#include <vector>

#include "../core/grid.hpp"
#include "../core/cell_t.hpp"
#include "../core/utility.hpp"

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

    std::vector<cell_t> IterativeSolver::solveArray(std::vector<cell_t> array, std::vector<int> hints)
    {
        // Get the minimum cell space required to satisfy the hints
        int minSpace = minimumSpaceFromHints(hints);
        int length = array.size();

        // Return a crossed array if space is 0.
        if (minSpace == 0)
        {
            return std::vector<cell_t>(length, CELL_CROSSED);
        }

        // Generate a new array to work on.
        std::vector<cell_t> result = std::vector<cell_t>(length, CELL_CLEARED);
        // Calculate the difference between the minimum required space and the total length of the array.
        int diff = length - minSpace;

        // Skip the first `diff` cells, at it is an area of uncertainty.
        // Set the beginning index at the cell after the first `diff` ones.
        int index = diff;

        // Iterate over each hint:
        for (auto hint = hints.begin(); hint != hints.end(); hint++)
        {
            // `hint` - `diff` cells need to be checked from the current index.
            int nCheck = *hint - diff;
            // Check said cells.
            for (int j = index; j < index + nCheck; j++)
            {
                result[j] = CELL_CHECKED;
            }
            // Update index.
            index += nCheck;

            // `diff` + 1 cells need to be skipped before the next hints, just update the index.
            index = diff + 1;
        }

        // Note: all of the above operation could be considered risky, as the index value is never checked on before being used.
        // However, the functions and logic used to compute that index value should be 100% fail proof.
        // Therefore, if something fails, everything MUST fail. Trying to catch errors is pointless in this context.

        return result;
    }
}
