#ifndef COMMAND_SEQUENCE_HPP
#define COMMAND_SEQUENCE_HPP

#include <vector>
#include <string>
#include <memory>

#include "../cli_command.hpp"
#include "../cli_state.hpp"

namespace Picross
{
    class CommandSequence : public CLICommand
    {
        private:    // Attributes
            std::vector<std::shared_ptr<CLICommand>> _commandList;
            std::string _tooltip;

        public:     // Public methods
            CommandSequence(std::vector<std::shared_ptr<CLICommand>> commandList, std::string tooltip = "");
            virtual ~CommandSequence();
            
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//COMMAND_SEQUENCE_HPP