#include "solve_grid_command.hpp"
#include "../cli_input.hpp"

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

    void SolveCommand::run(CLIState& state)
    {
        state.out() << "Coming soon." << std::endl;
    }
}