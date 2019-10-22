#ifndef SOLVE_COMMAND_HPP
#define SOLVE_COMMAND_HPP

#include "../cli_command.hpp"
#include "../../solving/solver.hpp"

namespace Picross
{
    class SolveCommand : public CLICommand
    {
        public:
            SolveCommand();
            virtual std::string getTooltip();
            virtual void run(CLIState& state);

        private:
            void showSolvers(CLIState& state, std::vector<std::shared_ptr<Solver>>& solvers);
            void handleSolving(CLIState& state, std::shared_ptr<Solver> solver);
    };
}

#endif//SOLVE_COMMAND_HPP