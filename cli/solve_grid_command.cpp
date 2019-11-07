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
        std::vector<std::shared_ptr<Solver>> solvers = instantiateAllSolvers();

        if (solvers.size())
        {
            while (true)
            {
                showSolvers(solvers, streams);
                int input = CLIInput::askForBoundedInput<int>("Please make a choice: ", 0, SOLVER_COUNT, streams);

                if (input == 0)
                {
                    break;
                }

                handleSolving(solvers[input], state, streams);
                break;
            }
        }
        else
        {
            streams.out() << "No solvers available!" << std::endl;
        }

        return COMMAND_SUCCESS;
    }

    void SolveCommand::showSolvers(std::vector<std::shared_ptr<Solver>>& solvers, CLIStreams& streams)
    {
        streams.out() << "Available solvers:\n";
        for (int i = 0; i < solvers.size(); i++)
        {
            streams.out() << i + 1 << ". " << solvers[i]->name() << '\n';
        }
        streams.out() << "0. Exit" << std::endl;
    }

    void SolveCommand::handleSolving(std::shared_ptr<Solver> solver, PicrossCLIState& state, CLIStreams& streams)
    {
        Grid grid = Grid(state.grid());

        streams.out() << "Solving grid using " << solver->name() << "..." << std::endl;
        solver->solve(grid);
        streams.out() << "Solving complete.\n";

        TextGridFormatter formatter;

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
    }
}