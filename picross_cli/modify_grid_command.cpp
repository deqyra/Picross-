#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "modify_grid_command.hpp"
#include "picross_cli_state.hpp"

#include "../tools/micro_shell/micro_shell.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"

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

    int ModifyGridCommand::run(PicrossCLIState& cliState, CLIStreams& streams)
    {
        streams.out() << "Invoking micro shell..." << std::endl;
        PicrossShell shell = instantiateMicroShell();
        PicrossShellState shellState = CLIStateToShellState(cliState);

        streams.out() << "Picross micro-shell. Type 'help' to get a list of available commands." << std::endl;
        shell.run(shellState, streams);

        cliState = shellStateToCLIState(shellState);

        return COMMAND_SUCCESS;
    }

    PicrossCLIState ModifyGridCommand::shellStateToCLIState(PicrossShellState& shellState)
    {
        PicrossCLIState cliState = PicrossCLIState();
        cliState.grid() = shellState.mainGrid();
        return cliState;
    }

    PicrossShellState ModifyGridCommand::CLIStateToShellState(PicrossCLIState& cliState)
    {
        PicrossShellState shellState = PicrossShellState();
        shellState.mainGrid() = cliState.grid();
        shellState.workingGrid() = cliState.grid();
        return shellState;
    }

    ModifyGridCommand::PicrossShell ModifyGridCommand::instantiateMicroShell()
    {
        ModifyGridCommand::PicrossShell shell = MicroShell<PicrossShellState>();
        return shell;
    }
}