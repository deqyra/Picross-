#include "solve_command.hpp"
#include "../cli_input.hpp"
#include "../../io/xml_grid_serializer.hpp"

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