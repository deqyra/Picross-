#ifndef PICROSS_SHELL__PICROSS_SHELL_CLEAR_COMMAND_HPP
#define PICROSS_SHELL__PICROSS_SHELL_CLEAR_COMMAND_HPP

#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "picross_shell_state.hpp"

#include <string>

namespace Picross
{
    class PicrossShellClearCommand : public MicroShellCommand<PicrossShellState>
    {
        public:     // Public methods
            PicrossShellClearCommand();
            virtual ~PicrossShellClearCommand();

            virtual int processInput(std::string command, PicrossShellState& state, CLIStreams& streams = CLIInput::defaultStreams);
            virtual std::string name();
            virtual std::string description();
            virtual std::string help();
    };
}

#endif//PICROSS_SHELL__PICROSS_SHELL_CLEAR_COMMAND_HPP