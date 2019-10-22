#ifndef CREATE_GRID_COMMAND_HPP
#define CREATE_GRID_COMMAND_HPP

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
            virtual void run(CLIState& state);

        private:    // Private methods
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, std::string numberedPrefixNoun, CLIState& state);
    };
}

#endif//CREATE_GRID_COMMAND_HPP