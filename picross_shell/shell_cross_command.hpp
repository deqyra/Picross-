#ifndef PICROSS_SHELL__SHELL_CROSS_COMMAND_HPP
#define PICROSS_SHELL__SHELL_CROSS_COMMAND_HPP

#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_state.hpp"

#include <string>

namespace Picross
{
    class ShellCrossCommand : public MicroShellCommand<PicrossShellState>
    {
        public:     // Public methods
            ShellCrossCommand();
            virtual ~ShellCrossCommand();

            virtual int processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams = CLIInput::defaultStreams);
            virtual std::string name();
            virtual std::string description();
            virtual std::string help();
    };
}

#endif//PICROSS_SHELL__SHELL_CROSS_COMMAND_HPP