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
            // Handle user input for all hints in a certain direction.
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIStreams& streams);
            // Check whether hints are all positive.
            bool hintsArePositiveNonZero(std::vector<int>& hints);
    };
}

#endif//CLI__CREATE_GRID_COMMAND_HPP