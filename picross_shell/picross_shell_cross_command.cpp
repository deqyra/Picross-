#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_cross_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "cell_manip_for_commands.hpp"
#include "../tools/string_tools.hpp"
#include "../io/text_grid_formatter.hpp"
#include "../tools/exceptions/range_bounds_exceeded_error.hpp"

namespace Picross
{
    PicrossShellCrossCommand::PicrossShellCrossCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    PicrossShellCrossCommand::~PicrossShellCrossCommand()
    {

    }

    int PicrossShellCrossCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 3)          // Too few arguments
        {
            streams.out() << "cross: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() > 3)     // Too many arguments
        {
            streams.out() << "cross: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else // if (tokens.size() == 3)
        {
            return handleCellManipForCommand(tokens, CELL_CROSSED, "cross", state, streams);
        }
    }

    std::string PicrossShellCrossCommand::name()
    {
        return "cross";
    }

    std::string PicrossShellCrossCommand::description()
    {
        return "Cross cell or range of cells";
    }

    std::string PicrossShellCrossCommand::help()
    {
        std::string s;
        s += "cross - crosses cells in the working grid.\n";
        s += "Syntax: cross <i|i0:in> <j|j0:jn>\n";
        s += " - i: row coordinate of a cell to cross.\n";
        s += " - i0:in: row coordinate range of cells to cross (e.g. 1:4).\n";
        s += " - j: column coordinate of a cell to cross.\n";
        s += " - j0:jn: column coordinate range of cells to cross (e.g. 2:7).\n";
        return s;
    }

}
