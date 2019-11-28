#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_display_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"
#include "../io/text_grid_formatter.hpp"

namespace Picross
{
    ShellDisplayCommand::ShellDisplayCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellDisplayCommand::~ShellDisplayCommand()
    {

    }

    int ShellDisplayCommand::processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams)
    {
        // Expected syntax: see docstring in help().
        
        // Parse arguments.
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        TextGridFormatter tgf = TextGridFormatter();

        if (tokens.size() > 2)          // Too many arguments.
        {
            streams.out() << "display: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() < 2)     // No arguments (apart from the command name).
        {
            streams.out() << tgf.renderGridWithHints(state.workingGrid());
            return SHELL_COMMAND_SUCCESS;
        }
        else // if (tokens.size() == 2)
        {
            if (tokens[1] != "nohints")
            {
                streams.out() << "display: unknown argument \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }

            // else if (tokens[1] == "nohints")
            streams.out() << tgf.renderGrid(state.workingGrid());
            return SHELL_COMMAND_SUCCESS;
        }
    }

    std::string ShellDisplayCommand::name()
    {
        return "display";
    }

    std::string ShellDisplayCommand::description()
    {
        return "Display grid in its current state";
    }

    std::string ShellDisplayCommand::help()
    {
        std::string s;
        s += "display - print a representation of the current state of the working grid.\n";
        s += "Syntax: display [nohints]\n";
        s += " - `nohints`: when specified, hints are not printed on the side of the grid.";
        return s;
    }

}
