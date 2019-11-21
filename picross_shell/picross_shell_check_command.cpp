#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_check_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "cell_manip_for_commands.hpp"
#include "../tools/string_tools.hpp"
#include "../io/text_grid_formatter.hpp"
#include "../tools/exceptions/range_bounds_exceeded_error.hpp"

namespace Picross
{
    PicrossShellCheckCommand::PicrossShellCheckCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    PicrossShellCheckCommand::~PicrossShellCheckCommand()
    {

    }

    int PicrossShellCheckCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 3)          // Too few arguments
        {
            streams.out() << "check: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() > 3)     // Too many arguments
        {
            streams.out() << "check: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else // if (tokens.size() == 3)
        {
            return handleCellManipForCommand(tokens, CELL_CHECKED, "check", state, streams);
        }
    }

    std::string PicrossShellCheckCommand::name()
    {
        return "check";
    }

    std::string PicrossShellCheckCommand::description()
    {
        return "Check cell or range of cells";
    }

    std::string PicrossShellCheckCommand::help()
    {
        std::string s;
        s += "check - checks cells in the working grid.\n";
        s += "Syntax: check <i|i0:in> <j|j0:jn>\n";
        s += " - i: row coordinate of a cell to check.\n";
        s += " - i0:in: row coordinate range of cells to check (e.g. 3:6).\n";
        s += " - j: column coordinate of a cell to check.\n";
        s += " - j0:jn: column coordinate range of cells to check (e.g. 8:9).\n";
        return s;
    }

}
