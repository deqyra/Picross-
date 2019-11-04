#ifndef CLI__COMMANDS__SAVE_GRID_COMMAND_HPP
#define CLI__COMMANDS__SAVE_GRID_COMMAND_HPP

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
            virtual int run(CLIState& state);
    };
}

#endif//CLI__COMMANDS__SAVE_GRID_COMMAND_HPP