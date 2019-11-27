#include "../tools/cli/cli_command.hpp"
#include "solve_grid_command.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "../tools/cli/cli_input.hpp"
#include "picross_cli_state.hpp"
#include "../solving/solver.hpp"
#include "../solving/utility.hpp"
#include "../io/text_grid_formatter.hpp"

namespace Picross
{
    SolveCommand::SolveCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    SolveCommand::~SolveCommand()
    {
        
    }

    std::string SolveCommand::getTooltip()
    {
        return "Solve grid";
    }

    int SolveCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        std::vector<SolverPtr> solvers = instantiateAllSolvers();

        if (solvers.size())
        {
            // Allow choosing from available solvers if any.
            showSolvers(solvers, streams);
            int input = CLIInput::askForBoundedInput<int>("Please make a choice: ", 0, SOLVER_COUNT, streams);

            // In case of exit, return immediately.
            if (input == 0)
            {
                return COMMAND_SUCCESS;
            }

            // Otherwise, solve.
            bool result = handleSolving(solvers[input], state, streams);
            return result ? COMMAND_SUCCESS : COMMAND_FAILURE;            
        }
        else
        {
            // Print an informative message if no solvers are available.
            streams.out() << "No solvers available!" << std::endl;
        }

        return COMMAND_SUCCESS;
    }

    void SolveCommand::showSolvers(std::vector<SolveCommand::SolverPtr>& solvers, CLIStreams& streams)
    {
        // Print available solvers in a numbered list.
        streams.out() << "Available solvers:\n";
        for (int i = 0; i < solvers.size(); i++)
        {
            streams.out() << i + 1 << ". " << solvers[i]->name() << '\n';
        }
        streams.out() << "0. Exit" << std::endl;
    }

    bool SolveCommand::handleSolving(SolveCommand::SolverPtr solver, PicrossCLIState& state, CLIStreams& streams)
    {
        // Create a working grid, in order not to fuck up the state grid if something goes horribly wrong.
        Grid grid = Grid(state.grid());

        streams.out() << "Solving grid using " << solver->name() << "..." << std::endl;

        // Do the actual solving.
        try
        {
            solver->solve(grid);
        }
        catch (const std::exception& e)
        {
            // Informative error logging.
            streams.err() << "Exception thrown by solver:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Solving aborted. Grid was not modified." << std::endl;

            return false;
        }

        streams.out() << "Solving complete.\n";

        TextGridFormatter formatter;

        // Print computed solution, and ask whether to commit the results to the state.
        streams.out() << "The grid is now in the following state:\n";
        streams.out() << formatter.renderGridWithHints(grid, true);
        bool save = CLIInput::askForInput<bool>("Do you want to keep these changes? ", streams);

        if (save)
        {
            state.grid() = grid;
            streams.out() << "Changes saved to current state." << std::endl;
        }
        else
        {
            streams.out() << "Changes discarded." << std::endl;
        }

        return true;
    }
}