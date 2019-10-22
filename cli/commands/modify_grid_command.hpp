#ifndef MODIFY_GRID_COMMAND_HPP
#define MODIFY_GRID_COMMAND_HPP

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

#endif//MODIFY_GRID_COMMAND_HPP