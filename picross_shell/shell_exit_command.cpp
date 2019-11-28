#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_exit_command.hpp"
#include "picross_shell_state.hpp"

#include <string>

namespace Picross
{
    ShellExitCommand::ShellExitCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellExitCommand::~ShellExitCommand()
    {

    }

    int ShellExitCommand::processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams)
    {
        // No argument checking: if exit was invoked, start the exit procedure.
        
        if (state.mainGrid() != state.workingGrid())
        {
            streams.out() << "You have pending changes on your working grid. Exiting the shell will discard them.\n";
            streams.out() << "Use 'commit' to save those changes to the main grid." << std::endl;
            bool exit = CLIInput::askForInput<bool>("Do you want to exit the shell and discard changes? ", streams);

            return exit ? SHELL_EXIT : SHELL_COMMAND_SUCCESS;
        }
        return SHELL_EXIT;
    }

    std::string ShellExitCommand::name()
    {
        return "exit";
    }

    std::string ShellExitCommand::description()
    {
        return "Exit the shell";
    }

    std::string ShellExitCommand::help()
    {
        std::string s;
        s += "exit - exit the shell.\n";
        s += "No arguments.\n";
        return s;
    }

}
