#include "cell_manip_for_commands.hpp"

#include <stdexcept>
#include <vector>
#include <string>
#include "picross_shell_state.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/string_tools.hpp"
#include "../tools/exceptions/range_bounds_exceeded_error.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"

namespace Picross
{
    int handleCellManipForCommand(std::vector<std::string> tokens, cell_t value, std::string commandName, PicrossShellState& state, CLIStreams& streams)
    {
        int i0, in;
        int j0, jn;
        bool success;
        int height = state.workingGrid().getHeight();
        int width = state.workingGrid().getWidth();

        try
        {
            StringTools::parseIntRange(tokens[1], ':', i0, in, 0, height - 1, true);
        }
        catch (std::invalid_argument& e)
        {
            streams.out() << commandName << ": range \"" << tokens[1] << "\" could not be parsed." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        catch (RangeBoundsExceededError& e)
        {
            streams.out() << commandName << ": row range (" << tokens[1] << ") is out of bounds for grid of height " << height << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        try
        {
            StringTools::parseIntRange(tokens[2], ':', j0, jn, 0, width - 1, true);
        }
        catch (std::invalid_argument& e)
        {
            streams.out() << commandName << ": range \"" << tokens[2] << "\" could not be parsed." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }
        catch (RangeBoundsExceededError& e)
        {
            streams.out() << commandName << ": column range (" << tokens[2] << ") is out of bounds for grid of width " << width << "." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        state.workingGrid().setCellRange(i0, in, j0, jn, value);

        return SHELL_COMMAND_SUCCESS;
    }
}