#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "modify_grid_command.hpp"
#include "picross_cli_state.hpp"

#include "../tools/micro_shell/micro_shell.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"

#include "../picross_shell/picross_shell_state.hpp"
#include "../picross_shell/picross_shell_check_command.hpp"
#include "../picross_shell/picross_shell_cross_command.hpp"
#include "../picross_shell/picross_shell_clear_command.hpp"
#include "../picross_shell/picross_shell_commit_command.hpp"
#include "../picross_shell/picross_shell_rollback_command.hpp"
#include "../picross_shell/picross_shell_display_command.hpp"
#include "../picross_shell/picross_shell_hints_command.hpp"
#include "../picross_shell/picross_shell_exit_command.hpp"

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
        shell.addCommand(std::make_shared<PicrossShellCheckCommand>());
        shell.addCommand(std::make_shared<PicrossShellCrossCommand>());
        shell.addCommand(std::make_shared<PicrossShellClearCommand>());
        shell.addCommand(std::make_shared<PicrossShellCommitCommand>());
        shell.addCommand(std::make_shared<PicrossShellRollbackCommand>());
        shell.addCommand(std::make_shared<PicrossShellDisplayCommand>());
        shell.addCommand(std::make_shared<PicrossShellHintsCommand>());
        shell.setExitCommand(std::make_shared<PicrossShellExitCommand>());
        return shell;
    }
}