#include "iterative_solver.hpp"

#include <string>
#include <vector>
#include <iostream>

#include "../core/grid.hpp"
#include "../core/cell_t.hpp"
#include "../core/utility.hpp"
#include "../core/exceptions/conflicting_merge_error.hpp"
#include "exceptions/solving_error.hpp"

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
        //
        // Start off with basic solving.
        //
        
        // First trivially solve the grid's rows.
        for (int i = 0; i < grid.getHeight(); i++)
        {
            std::vector<cell_t> result = arrayTrivialChecking(grid.getRow(i), grid.getRowHints(i));
            grid.setRow(i, result);
        }

        // Create a copy of the grid, and solve the grid's columns in it.
        Grid colCopy = grid;
        for (int j = 0; j < colCopy.getWidth(); j++)
        {
            std::vector<cell_t> result = arrayTrivialChecking(colCopy.getCol(j), colCopy.getColHints(j));
            colCopy.setCol(j, result);
        }

        // Attempt to merge column solving result back into the main grid.
        std::cout << "MERGED RESULTS:" << std::endl;
        try
        {
            grid.merge(colCopy, true);
        }
        catch(const ConflictingMergeError& e)
        {
            std::string s = "IterativeSolver: row and column hints give contradictory information. Grid cannot be solved.";
            throw SolvingError(s);
        }
        catch(const std::exception& e)
        {
            std::string s = "IterativeSolver: merging trivial row and col solutions failed:\n";
            s += e.what();
            throw SolvingError(s);
        }
        
        std::cout << grid << std::endl << std::endl << std::endl << std::endl;

        //
        // If the grid was not solved by this alone, start iterating.
        //

        // Keep track of the previous state.
        Grid lastState = grid;
        // Whether the grid is solved.
        bool solved = grid.isSolved();
        // Whether the solving is stalled (no changes after a full iteration).
        bool stalled = false;

        while (!solved && !stalled)
        {
            // Iterate.
            gridIteration(grid);
            break;

            // Update loop control.
            solved = grid.isSolved();
            stalled = (grid == lastState);
            // Register last state.
            lastState = grid;
        }
    }

    void IterativeSolver::gridIteration(Grid& grid)
    {
        for (int i = 0; i < grid.getHeight(); i++)
        {
            std::vector<cell_t> array = arrayIteration(grid.getRow(i), grid.getRowHints(i));
            grid.setRow(i, array);
        }

        for (int j = 0; j < grid.getHeight(); j++)
        {
            std::vector<cell_t> array = arrayIteration(grid.getCol(j), grid.getColHints(j));
            grid.setCol(j, array);
        }
    }

    std::vector<cell_t> IterativeSolver::arrayIteration(std::vector<cell_t> array, const std::vector<int>& hints)
    {
        array = arrayTrivialCrossing(array, hints);
        array = arrayForceCrossing(array, hints);
        array = arrayGluing(array, hints);
        array = arrayJoin(array, hints);
        array = arraySplit(array, hints);

        return array;
    }

    //
    // SOLVING UTILITIES
    //

    std::vector<cell_t> IterativeSolver::arrayTrivialChecking(std::vector<cell_t> array, std::vector<int> hints)
    {
        // Get the minimum cell space required to satisfy the hints
        int minSpace = minimumSpaceFromHints(hints);
        int length = array.size();

        // Return a crossed array if space is 0.
        if (minSpace == 0)
        {
            return std::vector<cell_t>(length, CELL_CROSSED);
        }

        // Calculate the difference between the minimum required space and the total length of the array.
        int diff = length - minSpace;
        
        // Generate a new array to work on.
        std::vector<cell_t> result;
        // If the hints fit exactly in the length of the array, then cells that are not checked are CROSSED.
        if (minSpace == length)
        {
            result = std::vector<cell_t>(length, CELL_CROSSED);
        }
        // Otherwise they are simply empty.
        else
        {
            result = std::vector<cell_t>(length, CELL_CLEARED);
        }
        
        // The following algorithm works when the hints fit exactly as well as when they don't.

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
            index += diff + 1;
        }

        // Note: all of the above operation could be considered risky, as the index value is never checked on before being used.
        // However, the functions and logic used to compute that index value should be 100% fail proof.
        // Therefore, if something fails, everything MUST fail. Trying to catch errors is pointless in this context.

        return result;
    }

    std::vector<cell_t> IterativeSolver::arrayTrivialCrossing(std::vector<cell_t> array, std::vector<int> hints)
    {

    }

    std::vector<cell_t> IterativeSolver::arrayForceCrossing(std::vector<cell_t> array, std::vector<int> hints)
    {

    }

    std::vector<cell_t> IterativeSolver::arrayGluing(std::vector<cell_t> array, std::vector<int> hints)
    {

    }

    std::vector<cell_t> IterativeSolver::arrayJoin(std::vector<cell_t> array, std::vector<int> hints)
    {

    }

    std::vector<cell_t> IterativeSolver::arraySplit(std::vector<cell_t> array, std::vector<int> hints)
    {

    }
}
