#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "cli_modify_grid_command.hpp"
#include "picross_cli_state.hpp"

#include "../tools/micro_shell/micro_shell.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"

#include "../picross_shell/picross_shell_state.hpp"
#include "../picross_shell/shell_check_command.hpp"
#include "../picross_shell/shell_cross_command.hpp"
#include "../picross_shell/shell_clear_command.hpp"
#include "../picross_shell/shell_commit_command.hpp"
#include "../picross_shell/shell_rollback_command.hpp"
#include "../picross_shell/shell_display_command.hpp"
#include "../picross_shell/shell_hints_command.hpp"
#include "../picross_shell/shell_is_solved_command.hpp"
#include "../picross_shell/shell_hint_coherence_command.hpp"
#include "../picross_shell/shell_exit_command.hpp"

#include <string>
#include <iostream>

namespace Picross
{
    CLIModifyGridCommand::CLIModifyGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    CLIModifyGridCommand::~CLIModifyGridCommand()
    {
        
    }

    std::string CLIModifyGridCommand::getTooltip()
    {
        return "Modify grid";
    }

    int CLIModifyGridCommand::run(PicrossCLIState& cliState, CLIStreams& streams)
    {
        // Instantiate the shell and create state to work on.
        streams.out() << "Invoking micro-shell..." << std::endl;
        PicrossShell shell = instantiateMicroShell();
        PicrossShellState shellState = CLIStateToShellState(cliState);

        bool cleanExit = false;

        while (!cleanExit)
        {
            try
            {
                // Run the shell.
                streams.out() << "Picross micro-shell. Type 'help' to get a list of available commands." << std::endl;
                shell.run(shellState, streams);
                cleanExit = true;
            }
            catch(const std::exception& e)
            {
                // Informative error logging.
                streams.err() << "The micro-shell threw an unhandled exception:\n";
                streams.err() << e.what() << '\n';
                streams.out() << "Restarting..." << std::endl;
            }
        }
        // The shell is done running.

        // Retrieve modified grid.
        cliState = shellStateToCLIState(shellState);

        return CLI_COMMAND_SUCCESS;
    }

    PicrossCLIState CLIModifyGridCommand::shellStateToCLIState(PicrossShellState& shellState)
    {
        // Keep only the main grid (discarding potential pending changes).
        PicrossCLIState cliState = PicrossCLIState();
        cliState.grid() = shellState.mainGrid();
        return cliState;
    }

    PicrossShellState CLIModifyGridCommand::CLIStateToShellState(PicrossCLIState& cliState)
    {
        // Copy the grid in CLI in both working grids of the shell state.
        PicrossShellState shellState = PicrossShellState();
        shellState.mainGrid() = cliState.grid();
        shellState.workingGrid() = cliState.grid();
        return shellState;
    }

    CLIModifyGridCommand::PicrossShell CLIModifyGridCommand::instantiateMicroShell()
    {
        // Add all known commands to the shell.
        CLIModifyGridCommand::PicrossShell shell = MicroShell<PicrossShellState>();
        shell.addCommand(std::make_shared<ShellCheckCommand>());
        shell.addCommand(std::make_shared<ShellCrossCommand>());
        shell.addCommand(std::make_shared<ShellClearCommand>());
        shell.addCommand(std::make_shared<ShellCommitCommand>());
        shell.addCommand(std::make_shared<ShellRollbackCommand>());
        shell.addCommand(std::make_shared<ShellDisplayCommand>());
        shell.addCommand(std::make_shared<ShellHintsCommand>());
        shell.addCommand(std::make_shared<ShellIsSolvedCommand>());
        shell.addCommand(std::make_shared<ShellHintCoherenceCommand>());
        shell.setExitCommand(std::make_shared<ShellExitCommand>());
        return shell;
    }
} // namespace Picross