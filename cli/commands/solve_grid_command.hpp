#ifndef CLI__COMMANDS__SOLVE_COMMAND_HPP
#define CLI__COMMANDS__SOLVE_COMMAND_HPP

#include "../cli_command.hpp"

#include <string>
#include <vector>
#include <memory>

#include "../cli_state.hpp"
#include "../../solving/solver.hpp"

namespace Picross
{
    class SolveCommand : public CLICommand
    {
        public:     // Public methods
            SolveCommand();
            virtual ~SolveCommand();

            virtual std::string getTooltip();
            virtual int run(CLIState& state);

        private:    // Private methods
            void showSolvers(CLIState& state, std::vector<std::shared_ptr<Solver>>& solvers);
            void handleSolving(CLIState& state, std::shared_ptr<Solver> solver);
    };
}

#endif//CLI__COMMANDS__SOLVE_COMMAND_HPP