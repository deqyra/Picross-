#ifndef SOLVE_COMMAND_HPP
#define SOLVE_COMMAND_HPP

#include "../cli_command.hpp"

namespace Picross
{
    class SolveCommand : public CLICommand
    {
        public:
            SolveCommand();
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//SOLVE_COMMAND_HPP