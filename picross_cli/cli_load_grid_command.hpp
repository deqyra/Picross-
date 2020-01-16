#ifndef PICROSS_CLI__CLI_LOAD_GRID_COMMAND_HPP
#define PICROSS_CLI__CLI_LOAD_GRID_COMMAND_HPP

#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "picross_cli_state.hpp"

#include <string>

namespace Picross
{
    class CLILoadGridCommand : public CLICommand<PicrossCLIState>
    {
        public:     // Public methods
            CLILoadGridCommand();
            virtual ~CLILoadGridCommand();

            virtual std::string getTooltip();
            virtual int run(PicrossCLIState& state, CLIStreams& streams = CLIInput::defaultStreams);
    };
} // namespace Picross

#endif//PICROSS_CLI__CLI_LOAD_GRID_COMMAND_HPP