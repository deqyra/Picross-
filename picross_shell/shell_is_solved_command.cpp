#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_is_solved_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"

namespace Picross
{
    ShellIsSolvedCommand::ShellIsSolvedCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellIsSolvedCommand::~ShellIsSolvedCommand()
    {

    }

    int ShellIsSolvedCommand::processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() > 1)          // Too many arguments.
        {
            streams.out() << "rollback: no arguments expected." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Revert changes on the working grid.
        streams.out() << (state.workingGrid().isSolved() ? "true" : "false") << std::endl;
        return SHELL_COMMAND_SUCCESS;
    }

    std::string ShellIsSolvedCommand::name()
    {
        return "isSolved";
    }

    std::string ShellIsSolvedCommand::description()
    {
        return "Tell whether the grid is solved";
    }

    std::string ShellIsSolvedCommand::help()
    {
        std::string s;
        s += "isSolved - tell whether the current state of the grid satisfies its hints.\n";
        s += "No arguments.\n";
        return s;
    }

}
