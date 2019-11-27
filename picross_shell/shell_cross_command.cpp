#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_cross_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "cell_manip_for_commands.hpp"
#include "../tools/string_tools.hpp"
#include "../io/text_grid_formatter.hpp"
#include "../tools/exceptions/range_bounds_exceeded_error.hpp"

namespace Picross
{
    ShellCrossCommand::ShellCrossCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellCrossCommand::~ShellCrossCommand()
    {

    }

    int ShellCrossCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        // Expected syntax: see docstring in help().
        
        // Parse arguments.
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 3)          // Too few arguments.
        {
            streams.out() << "cross: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() > 3)     // Too many arguments.
        {
            streams.out() << "cross: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else // if (tokens.size() == 3)
        {
            return handleCellManipForCommand(tokens, CELL_CROSSED, "cross", state, streams);
        }
    }

    std::string ShellCrossCommand::name()
    {
        return "cross";
    }

    std::string ShellCrossCommand::description()
    {
        return "Cross cell or range of cells";
    }

    std::string ShellCrossCommand::help()
    {
        std::string s;
        s += "cross - cross cells in the working grid.\n";
        s += "Syntax: cross <i|i0:in> <j|j0:jn>\n";
        s += " - `i`: row coordinate of the cells to cross.\n";
        s += " - `i0:in`: row coordinate range of the cells to cross (e.g: `3:6`).\n";
        s += " - `j`: column coordinate of the cells to cross.\n";
        s += " - `j0:jn`: column coordinate range of the cells to cross (e.g: `8:9`).\n";
        s += "Omitting x0 or xn will automatically expand the range respectively to its lower bound or higher bound.\n";
        s += "E.g: `:3` will expand to `0:3`, `3:` will expand to `3:9` if 9 is the max value.\n";
        s += "`:` will expand to the whole range (continuing the above example, `0:9`).\n";
        return s;
    }

}
