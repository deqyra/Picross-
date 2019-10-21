#include "modify_grid_command.hpp"
#include "../cli_input.hpp"

namespace Picross
{
    ModifyGridCommand::ModifyGridCommand() :
        CLICommand()
    {

    }

    std::string ModifyGridCommand::getTooltip()
    {
        return "Modify grid";
    }

    void ModifyGridCommand::run(CLIState& state)
    {
        state.out() << "Coming soon." << std::endl;
    }
}