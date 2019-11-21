#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_hints_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include "../tools/string_tools.hpp"

namespace Picross
{
    PicrossShellHintsCommand::PicrossShellHintsCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    PicrossShellHintsCommand::~PicrossShellHintsCommand()
    {

    }

    int PicrossShellHintsCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 2)          // Too few arguments
        {
            streams.out() << "hints: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() == 2)
        {
            if (tokens[1] == "generate")
            {
                state.workingGrid().setHintsFromState();
                return SHELL_COMMAND_SUCCESS;
            }
            else if (tokens[1] == "h" || tokens[1] == "v")
            {
                streams.out() << "hints: too few arguments after \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
            else
            {
                streams.out() << "hints: unknown argument \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
        }
        else if (tokens.size() == 3)
        {
            if (tokens[1] == "generate")
            {
                streams.out() << "hints: unknown argument \"" << tokens[2] << "\" after \"generate\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
            else if (tokens[1] == "h" || tokens[1] == "v")
            {
                streams.out() << "hints: too few arguments after \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
            else
            {
                streams.out() << "hints: unknown argument \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
        }
        else if (tokens.size() == 4)
        {
            if (tokens[1] == "generate")
            {
                streams.out() << "hints: unknown argument \"" << tokens[2] << "\" after \"generate\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
            else if (tokens[1] == "h" || tokens[1] == "v")
            {
                return handleHintModification(tokens, state, streams);
            }
            else
            {
                streams.out() << "hints: unknown argument \"" << tokens[1] << "\"." << std::endl;
                return SHELL_COMMAND_BAD_ARGUMENTS;
            }
        }
        else
        {
            streams.out() << "hints: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        
        state.mainGrid() = state.workingGrid();
        return SHELL_COMMAND_SUCCESS;
    }

    std::string PicrossShellHintsCommand::name()
    {
        return "hints";
    }

    std::string PicrossShellHintsCommand::description()
    {
        return "Change grid hints";
    }

    std::string PicrossShellHintsCommand::help()
    {
        std::string s;
        s += "hints - modify hints on the working grid.\n";
        s += "Syntax 1: hints generate\n";
        s += " - Automatically generate hints from current grid state.\n";
        s += "Syntax 2: hints <h|v> <n> <values>\n";
        s += " - <h|v>: determines whether to affect horizontal (row) or vertical (column) hints.\n";
        s += " - <n>: 0-based index of the target row or column.\n";
        s += " - <values>: semicolon-separated positive integers representing the new hints.\n";
        return s;
    }

    int PicrossShellHintsCommand::handleHintModification(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Syntax: hints <h|v>       <indexToModify> <spaceSeparatedValues>
        // Syntax: hints <direction> <indexString>   <valueString>

        std::string direction = tokens[1];
        std::string indexString = tokens[2];
        std::string valueString = tokens[3];

        if (!StringTools::stringIsNum(indexString, false))
        {
            streams.out() << "\"" << indexString << "\" is not a valid index value." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        int index = std::stoi(indexString);
        
        // If index is out of bounds...
        if (direction == "h" && index >= state.workingGrid().getHeight())
        {
            streams.out() << "hints: index value " << index << " is out of bounds for grid of height " << state.workingGrid().getHeight() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        if (direction == "v" && index >= state.workingGrid().getWidth())
        {
            streams.out() << "hints: index value " << index << " is out of bounds for grid of width " << state.workingGrid().getWidth() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        std::vector<int> hints;
        try
        {
            hints = StringTools::stringToIntVector(valueString, PICROSS_SHELL_HINTS_COMMAND_DELIMITER);
        }
        catch (const std::invalid_argument& e)
        {
            streams.out() << "hints: hint values \"" << valueString << "\" could not be parsed." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        if (direction == "h" && !state.workingGrid().areValidRowHints(hints))
        {
            streams.out() << "hints: hint values \"" << valueString << "\" do not fit in grid of width " << state.workingGrid().getWidth() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        if (direction == "v" && !state.workingGrid().areValidColHints(hints))
        {
            streams.out() << "hints: hint values \"" << valueString << "\" do not fit in grid of height " << state.workingGrid().getHeight() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        if (direction == "h")
        {
            state.workingGrid().setRowHints(index, hints);
        }
        if (direction == "v")
        {
            state.workingGrid().setColHints(index, hints);
        }
        return SHELL_COMMAND_SUCCESS;
    }
}
