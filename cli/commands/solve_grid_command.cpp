#include "solve_grid_command.hpp"
#include "../cli_input.hpp"
#include "../../solving/solver.hpp"
#include "../../solving/utility.hpp"
#include "../../io/text_grid_formatter.hpp"

#include <vector>
#include <memory>

namespace Picross
{
    SolveCommand::SolveCommand() :
        CLICommand()
    {

    }

    std::string SolveCommand::getTooltip()
    {
        return "Solve grid";
    }

    void SolveCommand::run(CLIState &state)
    {
        std::vector<std::shared_ptr<Solver>> solvers = instantiateAllSolvers();

        if (solvers.size())
        {
            while (true)
            {
                showSolvers(state, solvers);
                int input = CLIInput::askForInput<int>("Which one do you want to use? ", state);
                if (input < 0 || input > solvers.size())
                {
                    state.err() << "Invalid input, please enter an integer between 0 and " << solvers.size() << "." << std::endl << std::endl; 
                    continue;
                }

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

        for (auto it = solvers.begin(); it != solvers.end(); it++)
        {
            delete (*it);
        }
    }

    void SolveCommand::showSolvers(CLIState& state, std::vector<std::shared_ptr<Solver>>& solvers)
    {
        state.out() << "Available solvers: " << std::endl;
        for (int i = 0; i < solvers.size(); i++)
        {
            state.out() << i + 1 << ". " << solvers[i]->name() << std::endl;
        }
        state.out() << "0. Exit" << std::endl;
        state.out() << std::endl;
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