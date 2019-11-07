#ifndef CLI__CREATE_GRID_COMMAND_HPP
#define CLI__CREATE_GRID_COMMAND_HPP

#include "../tools/cli/cli_command.hpp"
#include "picross_cli_state.hpp"

#include <vector>
#include <string>

namespace Picross
{
    class CreateGridCommand : public CLICommand<PicrossCLIState>
    {
        public:     // Public methods
            CreateGridCommand();
            virtual ~CreateGridCommand();

            virtual std::string getTooltip();
            virtual int run(PicrossCLIState& state, CLIStreams& streams = CLIInput::defaultStreams);

        private:    // Private methods
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIStreams& streams);
            bool hintsAreValid(std::vector<int>& hints);
    };
}

#endif//CLI__CREATE_GRID_COMMAND_HPP