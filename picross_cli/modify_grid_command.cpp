#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "modify_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <string>
#include <iostream>


namespace Picross
{
    ModifyGridCommand::ModifyGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    ModifyGridCommand::~ModifyGridCommand()
    {
        
    }

    std::string ModifyGridCommand::getTooltip()
    {
        return "Modify grid";
    }

    int ModifyGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        streams.out() << "Coming soon." << std::endl;
        return COMMAND_SUCCESS;
    }
}