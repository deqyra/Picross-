#ifndef CREATE_GRID_COMMAND_HPP
#define CREATE_GRID_COMMAND_HPP

#include <vector>
#include <string>

#include "../cli_command.hpp"

namespace Picross
{
    class CreateGridCommand : public CLICommand
    {
        public:
            CreateGridCommand();

            virtual std::string getTooltip();
            virtual void run(CLIState& state);

        private:
            std::vector<std::vector<int>> handleHintSequenceInput(int sequenceLength, std::string numberedPrefixNoun, CLIState& state);
    };
}

#endif//CREATE_GRID_COMMAND_HPP