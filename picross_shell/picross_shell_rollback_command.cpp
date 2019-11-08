#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_rollback_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"

namespace Picross
{
    PicrossShellRollbackCommand::PicrossShellRollbackCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    PicrossShellRollbackCommand::~PicrossShellRollbackCommand()
    {

    }

    int PicrossShellRollbackCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() > 1)          // Too many arguments
        {
            streams.out() << "rollback: no arguments expected." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        state.workingGrid() = state.mainGrid();
        return SHELL_COMMAND_SUCCESS;
    }

    std::string PicrossShellRollbackCommand::name()
    {
        return "rollback";
    }

    std::string PicrossShellRollbackCommand::description()
    {
        return "Discard pending changes";
    }

    std::string PicrossShellRollbackCommand::help()
    {
        std::string s;
        s += "rollback - discards pending changes and reverts the working grid to the same state as the main grid.\n";
        s += "No arguments.\n";
        return s;
    }

}
