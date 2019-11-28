#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "shell_hints_command.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>
#include <algorithm>
#include "../tools/string_tools.hpp"
#include "../tools/lambda_maker.hpp"

namespace Picross
{
    ShellHintsCommand::ShellHintsCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellHintsCommand::~ShellHintsCommand()
    {

    }

    int ShellHintsCommand::processInput(std::string command, PicrossShellState& state, CLIStreams& streams)
    {
        // Expected syntax: see docstring in help().

        // Parse arguments.
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() < 2)  // Too few arguments.
        {
            streams.out() << "hints: too few arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens[1] == "generate")   // Handle `hints generate ...` commands.
        {
            return generateSubroutine(tokens, state, streams);
        }
        else if (tokens[1] == "clear")      // Handle `hints clear ...` commands.
        {
            return clearSubroutine(tokens, state, streams);
        }
        else if (tokens[1] == "row" || tokens[1] == "col")  // Handle `hints <row|col> ...` commands.
        {
            return directionSubroutine(tokens, state, streams);
        }
        else    // Unknown argument.
        {
            streams.out() << "hints: unknown argument \"" << tokens[1] << "\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
    }

    int ShellHintsCommand::generateSubroutine(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Guarantees: - tokens.size() >= 2
        //             - tokens[1] == "generate"

        if (tokens.size() > 2)  // Too many arguments.
        {
            streams.out() << "hints: no arguments expected after \"generate\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Generate hints.
        state.workingGrid().setHintsFromState();
        return SHELL_COMMAND_SUCCESS;
    }

    int ShellHintsCommand::clearSubroutine(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Guarantees: - tokens.size() >= 2
        //             - tokens[1] == "clear"

        if (tokens.size() > 2)  // Too many arguments.
        {
            streams.out() << "hints: no arguments expected after \"clear\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Clear all hints.
        state.workingGrid().clearAllHints();
        return SHELL_COMMAND_SUCCESS;
    }

    int ShellHintsCommand::directionSubroutine(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Guarantees: - tokens.size() >= 2
        //             - tokens[1] == "row" || tokens[1] == "col"

        if (tokens.size() == 2)  // Too few arguments.
        {
            streams.out() << "hints: missing arguments after \"" + tokens[1] + "\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        if (tokens[2] == "clear")       // Handle `hints <row|col> clear` commands.
        {
            return clearDirectionSubroutine(tokens, state, streams);
        }
        else if (tokens.size() == 3)
        {
            streams.out() << "hints: unknown argument \"" + tokens[2] + "\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        else if (tokens.size() == 4)
        {
            return handleHintModification(tokens, state, streams);
        }
        else
        {
            streams.out() << "hints: too many arguments." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
    }

    int ShellHintsCommand::clearDirectionSubroutine(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Guarantees: - tokens.size() >= 2
        //             - tokens[1] == "row" || tokens[1] == "col"
        //             - tokens[2] == "clear"

        if (tokens.size() > 3)  // Too many arguments.
        {
            streams.out() << "hints: no arguments expected after \"clear\"." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Clear hints in either direction.
        if (tokens[1] == "row")
        {
            state.workingGrid().clearRowHints();
        }
        else // if (tokens[1] == "col")
        {
            state.workingGrid().clearColHints();
        }
        return SHELL_COMMAND_SUCCESS;
    }

    int ShellHintsCommand::handleHintModification(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams)
    {
        // Syntax:    hints <h|v>       <indexToModify> <spaceSeparatedValues>
        //                    ^               ^                ^
        //                    |               |                |
        //                    v               v                v
        // Variables: hints <direction> <indexString>   <valueString>

        std::string direction = tokens[1];
        std::string indexString = tokens[2];
        std::string valueString = tokens[3];

        // Direction is certified to be either "row" or "col", otherwise this function wouldn't have been called (unknown argument).
        // Check validity of index string.
        if (!StringTools::stringIsNum(indexString, false))
        {
            streams.out() << "hints: \"" << indexString << "\" is not a valid index value." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        int index = std::stoi(indexString);
        
        // If index is out of bounds, print a comprehensive error message.
        if (direction == "row" && index >= state.workingGrid().getHeight())
        {
            streams.out() << "hints: index value " << index << " is out of bounds for grid of height " << state.workingGrid().getHeight() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        if (direction == "col" && index >= state.workingGrid().getWidth())
        {
            streams.out() << "hints: index value " << index << " is out of bounds for grid of width " << state.workingGrid().getWidth() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // If hint sequence is "clear", reset hints for given index in given direction.
        if (valueString == "clear")
        {
            if (direction == "col")
            {
                state.workingGrid().setColHints(index, {});
            }
            else // if (direction == "row")
            {
                state.workingGrid().setRowHints(index, {});
            }

            return SHELL_COMMAND_SUCCESS;
        }

        // Attempt to parse new hints.
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

        // Checked parsed hints validity: all above 0.
        if (!std::all_of(hints.begin(), hints.end(), LambdaMaker::greater_than(0)))
        {
            streams.out() << "hints: hints must all be above 0." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Checked parsed hints validity: fit in current grid.
        if (direction == "row" && !state.workingGrid().areValidRowHints(hints))
        {
            streams.out() << "hints: hint values \"" << valueString << "\" do not fit in grid of width " << state.workingGrid().getWidth() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        if (direction == "col" && !state.workingGrid().areValidColHints(hints))
        {
            streams.out() << "hints: hint values \"" << valueString << "\" do not fit in grid of height " << state.workingGrid().getHeight() << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Set hints.
        if (direction == "row")
        {
            state.workingGrid().setRowHints(index, hints);
        }
        if (direction == "col")
        {
            state.workingGrid().setColHints(index, hints);
        }
        return SHELL_COMMAND_SUCCESS;
    }

    std::string ShellHintsCommand::name()
    {
        return "hints";
    }

    std::string ShellHintsCommand::description()
    {
        return "Change grid hints";
    }

    std::string ShellHintsCommand::help()
    {
        std::string s;
        s += "hints - modify hints on the working grid.\n";
        s += "Syntax 1: hints generate\n";
        s += " - Automatically generate hints from current grid state.\n";
        s += "Syntax 2: hints clear\n";
        s += " - Clear all hints in the grid.\n";
        s += "Syntax 3: hints <col|row> clear\n";
        s += " - Clear all hints in the given direction.\n";
        s += "Syntax 4: hints <col|row> <n> <values>\n";
        s += " - <col|row>: determines the direction of hints to change.\n";
        s += " - `n`: 0-based index of the target row or column.\n";
        s += " - `values`: semicolon-separated positive integers representing the new hints.\n";
        s += "If `values` is \"clear\", the hints for the given row or column are reset.\n";
        return s;
    }
}
