#ifndef LOAD_GRID_COMMAND_HPP
#define LOAD_GRID_COMMAND_HPP

#include "../cli_command.hpp"

#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    class LoadGridCommand : public CLICommand
    {
        public:     // Public methods
            LoadGridCommand();
            virtual ~LoadGridCommand();

            virtual std::string getTooltip();
            virtual int run(CLIState& state);
    };
}

#endif//LOAD_GRID_COMMAND_HPP