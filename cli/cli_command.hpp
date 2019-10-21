#ifndef CLI_COMMAND_HPP
#define CLI_COMMAND_HPP

#include <string>

#include "cli_state.hpp"

namespace Picross
{
    class CLICommand
    {
        public:
            CLICommand();
            virtual std::string getTooltip() = 0;
            virtual void run(CLIState& state) = 0;
    };
}

#endif//CLI_COMMAND_HPP