#ifndef CLI__COMMANDS__MODIFY_GRID_COMMAND_HPP
#define CLI__COMMANDS__MODIFY_GRID_COMMAND_HPP

#include "../cli_command.hpp"

#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    class ModifyGridCommand : public CLICommand
    {
        public:     // Public methods
            ModifyGridCommand();
            virtual ~ModifyGridCommand();

            virtual std::string getTooltip();
            virtual int run(CLIState& state);
    };
}

#endif//CLI__COMMANDS__MODIFY_GRID_COMMAND_HPP