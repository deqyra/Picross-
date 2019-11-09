#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_clear_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"
#include "../io/text_grid_formatter.hpp"

namespace Picross
{
    PicrossShellClearCommand::PicrossShellClearCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    PicrossShellClearCommand::~PicrossShellClearCommand()
    {

    }

    int PicrossShellClearCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 3)          // Too few arguments
        {
            streams.out() << "clear: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() > 3)     // Too many arguments
        {
            streams.out() << "clear: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else // if (tokens.size() == 3)
        {
            int i0, in;
            int j0, jn;
            bool success;
            
            success = StringTools::parseIntRange(tokens[1], ':', i0, in);
            if (!success)
            {
                streams.out() << "clear: range \"" << tokens[1] << "\" could not be parsed." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }

            success = StringTools::parseIntRange(tokens[2], ':', j0, jn);
            if (!success) return SHELL_COMMAND_BAD_ARGUMENTS;

            state.workingGrid().setCellRange(i0, in, j0, jn, CELL_CHECKED);

            return SHELL_COMMAND_SUCCESS;
        }
    }

    std::string PicrossShellClearCommand::name()
    {
        return "clear";
    }

    std::string PicrossShellClearCommand::description()
    {
        return "Clear cell or range of cells";
    }

    std::string PicrossShellClearCommand::help()
    {
        std::string s;
        s += "clear - clears cells in the working grid.\n";
        s += "Syntax: clear <i|i0:in> <j|j0:jn>\n";
        s += " - i: row coordinate of a cell to clear.\n";
        s += " - i0:in: row coordinate range of cells to clear (e.g. 1:4).\n";
        s += " - j: column coordinate of a cell to clear.\n";
        s += " - j0:jn: column coordinate range of cells to clear (e.g. 2:7).\n";
        return s;
    }

}
