#ifndef SAVE_GRID_COMMAND_HPP
#define SAVE_GRID_COMMAND_HPP

#include "../cli_command.hpp"

namespace Picross
{
    class SaveGridCommand : public CLICommand
    {
        public:
            SaveGridCommand();
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//SAVE_GRID_COMMAND_HPP