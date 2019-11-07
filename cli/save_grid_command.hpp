#ifndef CLI__SAVE_GRID_COMMAND_HPP
#define CLI__SAVE_GRID_COMMAND_HPP

#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "picross_cli_state.hpp"

#include <string>

namespace Picross
{
    class SaveGridCommand : public CLICommand<PicrossCLIState>
    {
        public:     // Public methods
            SaveGridCommand();
            virtual ~SaveGridCommand();

            virtual std::string getTooltip();
            virtual int run(PicrossCLIState& state, CLIStreams& streams = CLIInput::defaultStreams);
    };
}

#endif//CLI__SAVE_GRID_COMMAND_HPP