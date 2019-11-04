#ifndef CLI__COMMANDS__CREATE_GRID_COMMAND_HPP
#define CLI__COMMANDS__CREATE_GRID_COMMAND_HPP

#include "../cli_command.hpp"

#include <vector>
#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    class CreateGridCommand : public CLICommand
    {
        public:     // Public methods
            CreateGridCommand();
            virtual ~CreateGridCommand();

            virtual std::string getTooltip();
            virtual int run(CLIState& state);

        private:    // Private methods
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIState& state);
            bool hintsAreValid(std::vector<int>& hints);
    };
}

#endif//CLI__COMMANDS__CREATE_GRID_COMMAND_HPP