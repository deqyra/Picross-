#ifndef PICROSS_SHELL__PICROSS_SHELL_HINTS_COMMAND_HPP
#define PICROSS_SHELL__PICROSS_SHELL_HINTS_COMMAND_HPP

#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_state.hpp"

#include <vector>
#include <string>

// If you change this delimiter, don't forget to update the help() method.
#define PICROSS_SHELL_HINTS_COMMAND_DELIMITER ';'

namespace Picross
{
    class PicrossShellHintsCommand : public MicroShellCommand<PicrossShellState>
    {
        public:     // Public methods
            PicrossShellHintsCommand();
            virtual ~PicrossShellHintsCommand();

            virtual int processInput(std::string command, PicrossShellState& state, CLIStreams& streams = CLIInput::defaultStreams);
            virtual std::string name();
            virtual std::string description();
            virtual std::string help();

        private:    // Private methods
            // Handle hint modification based on command arguments.
            int handleHintModification(std::vector<std::string> tokens, PicrossShellState& state, CLIStreams& streams);
            // Check whether hints are all positive.
            bool hintsArePositiveNonZero(std::vector<int>& hints);
    };
}

#endif//PICROSS_SHELL__PICROSS_SHELL_HINTS_COMMAND_HPP