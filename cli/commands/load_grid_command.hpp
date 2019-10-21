#ifndef LOAD_GRID_COMMAND_HPP
#define LOAD_GRID_COMMAND_HPP

#include <string>

#include "../cli_command.hpp"

namespace Picross
{
    class LoadGridCommand : public CLICommand
    {
        public:
            LoadGridCommand();
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//LOAD_GRID_COMMAND_HPP