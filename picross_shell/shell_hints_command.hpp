#ifndef PICROSS_SHELL__SHELL_HINTS_COMMAND_HPP
#define PICROSS_SHELL__SHELL_HINTS_COMMAND_HPP

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
    class ShellHintsCommand : public MicroShellCommand<PicrossShellState>
    {
        public:     // Public methods
            ShellHintsCommand();
            virtual ~ShellHintsCommand();

            virtual int processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams = CLIInput::defaultStreams);
            virtual std::string name();
            virtual std::string description();
            virtual std::string help();

        private:    // Private methods
            // Handle hint modification based on command arguments.
            int handleHintModification(const std::vector<std::string>& tokens, PicrossShellState& state, CLIStreams& streams);
            // Handle `hints generate` commands
            int generateSubroutine(const std::vector<std::string>& tokens, PicrossShellState& state, CLIStreams& streams);
            // Handle `hints clear` commands
            int clearSubroutine(const std::vector<std::string>& tokens, PicrossShellState& state, CLIStreams& streams);
            // Handle `hints <row|col>` commands
            int directionSubroutine(const std::vector<std::string>& tokens, PicrossShellState& state, CLIStreams& streams);
            // Handle `hints <row|col> clear` commands
            int clearDirectionSubroutine(const std::vector<std::string>& tokens, PicrossShellState& state, CLIStreams& streams);
    };
} // namespace Picross

#endif//PICROSS_SHELL__SHELL_HINTS_COMMAND_HPP