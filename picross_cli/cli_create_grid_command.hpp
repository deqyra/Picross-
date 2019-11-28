#ifndef PICROSS_CLI__CLI_CREATE_GRID_COMMAND_HPP
#define PICROSS_CLI__CLI_CREATE_GRID_COMMAND_HPP

#include "../tools/cli/cli_command.hpp"
#include "picross_cli_state.hpp"

#include <vector>
#include <string>

namespace Picross
{
    class CLICreateGridCommand : public CLICommand<PicrossCLIState>
    {
        public:     // Public methods
            CLICreateGridCommand();
            virtual ~CLICreateGridCommand();

            virtual std::string getTooltip();
            virtual int run(PicrossCLIState& state, CLIStreams& streams = CLIInput::defaultStreams);

        private:    // Private methods
            // Handle user input for all hints in a certain direction.
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIStreams& streams);
    };
}

#endif//PICROSS_CLI__CLI_CREATE_GRID_COMMAND_HPP