#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_commit_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"

namespace Picross
{
    ShellCommitCommand::ShellCommitCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellCommitCommand::~ShellCommitCommand()
    {

    }

    int ShellCommitCommand::processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams)
    {
        // Expected syntax: see docstring in help().
        
        // Parse arguments.
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() > 1)          // Too many arguments.
        {
            streams.out() << "commit: no arguments expected." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        state.mainGrid() = state.workingGrid();
        return SHELL_COMMAND_SUCCESS;
    }

    std::string ShellCommitCommand::name()
    {
        return "commit";
    }

    std::string ShellCommitCommand::description()
    {
        return "Save pending changes";
    }

    std::string ShellCommitCommand::help()
    {
        std::string s;
        s += "commit - save pending changes from the working grid to the main grid.\n";
        s += "No arguments.\n";
        return s;
    }

}
