#ifndef CLI__CLI_COMMAND_HPP
#define CLI__CLI_COMMAND_HPP

#include <string>

#include "cli_state.hpp"

namespace Picross
{
    class CLICommand
    {
        public:
            virtual ~CLICommand();
            virtual std::string getTooltip() = 0;
            virtual int run(CLIState& state) = 0;
    };

    // Command exit codes
    inline static const int COMMAND_SUCCESS = 0;
    inline static const int COMMAND_FAILURE = -1;
}

#endif//CLI__CLI_COMMAND_HPP