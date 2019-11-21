#ifndef PICROSS_SHELL__CELL_MANIP_FOR_COMMANDS_HPP
#define PICROSS_SHELL__CELL_MANIP_FOR_COMMANDS_HPP

#include <vector>
#include <string>
#include "../core/cell_t.hpp"
#include "picross_shell_state.hpp"
#include "../tools/cli/cli_streams.hpp"

namespace Picross
{
    int handleCellManipForCommand(std::vector<std::string> tokens, cell_t value, std::string commandName, PicrossShellState& state, CLIStreams& streams);
}

#endif//PICROSS_SHELL__CELL_MANIP_FOR_COMMANDS_HPP