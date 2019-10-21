#ifndef MODIFY_GRID_COMMAND_HPP
#define MODIFY_GRID_COMMAND_HPP

#include "../cli_command.hpp"

namespace Picross
{
    class ModifyGridCommand : public CLICommand
    {
        public:
            ModifyGridCommand();
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//MODIFY_GRID_COMMAND_HPP