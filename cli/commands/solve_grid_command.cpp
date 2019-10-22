#include "../cli_command.hpp"
#include "solve_grid_command.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "../cli_input.hpp"
#include "../cli_state.hpp"
#include "../../solving/solver.hpp"
#include "../../solving/utility.hpp"
#include "../../io/text_grid_formatter.hpp"

namespace Picross
{
    SolveCommand::SolveCommand() :
        CLICommand()
    {

    }

    SolveCommand::~SolveCommand()
    {
        
    }

    std::string SolveCommand::getTooltip()
    {
        return "Solve grid";
    }

    int SolveCommand::run(CLIState& state)
    {
        std::vector<std::shared_ptr<Solver>> solvers = instantiateAllSolvers();

        if (solvers.size())
        {
            while (true)
            {
                showSolvers(state, solvers);
                int input = CLIInput::askForBoundedInput<int>("Please make a choice: ", state, 0, SOLVER_COUNT);

                if (input == 0)
                {
                    break;
                }

                handleSolving(state, solvers[input]);
                break;
            }
        }
        else
        {
            state.out() << "No solvers available!" << std::endl;
        }

        return COMMAND_SUCCESS;
    }

    void SolveCommand::showSolvers(CLIState& state, std::vector<std::shared_ptr<Solver>>& solvers)
    {
        state.out() << "Available solvers: " << std::endl;
        for (int i = 0; i < solvers.size(); i++)
        {
            state.out() << i + 1 << ". " << solvers[i]->name() << std::endl;
        }
        state.out() << "0. Exit" << std::endl;
    }

    void SolveCommand::handleSolving(CLIState& state, std::shared_ptr<Solver> solver)
    {
        Grid grid = Grid(state.grid());

        state.out() << "Solving grid using " << solver->name() << "..." << std::endl;
        solver->solve(grid);
        state.out() << "Solving complete.";

        TextGridFormatter formatter;

        state.out() << "The grid is now in the following state:" << std::endl;
        state.out() << formatter.renderGridWithHints(grid, true);
        bool save = CLIInput::askForInput<bool>("Do you want to keep these changes? ", state);

        if (save)
        {
            state.grid() = grid;
            state.out() << "Changes saved to current state." << std::endl;
        }
        else
        {
            state.out() << "Changes discarded." << std::endl;
        }
    }
}