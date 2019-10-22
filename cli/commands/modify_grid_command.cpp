#include "../cli_command.hpp"
#include "modify_grid_command.hpp"

#include <string>
#include <iostream>

#include "../cli_input.hpp"

namespace Picross
{
    ModifyGridCommand::ModifyGridCommand() :
        CLICommand()
    {

    }

    ModifyGridCommand::~ModifyGridCommand()
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