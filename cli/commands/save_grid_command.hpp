#ifndef SAVE_GRID_COMMAND_HPP
#define SAVE_GRID_COMMAND_HPP

#include "../cli_command.hpp"

#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    class SaveGridCommand : public CLICommand
    {
        public:     // Public methods
            SaveGridCommand();
            virtual ~SaveGridCommand();

            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//SAVE_GRID_COMMAND_HPP