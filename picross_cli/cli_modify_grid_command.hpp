#ifndef PICROSS_CLI__CLI_MODIFY_GRID_COMMAND_HPP
#define PICROSS_CLI__CLI_MODIFY_GRID_COMMAND_HPP

#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "picross_cli_state.hpp"
#include "../tools/micro_shell/micro_shell.hpp"
#include "../picross_shell/picross_shell_state.hpp"

#include <string>

namespace Picross
{
    class CLIModifyGridCommand : public CLICommand<PicrossCLIState>
    {
        using PicrossShell = MicroShell<PicrossShellState>;

        public:     // Public methods
            CLIModifyGridCommand();
            virtual ~CLIModifyGridCommand();

            virtual std::string getTooltip();
            virtual int run(PicrossCLIState& state, CLIStreams& streams = CLIInput::defaultStreams);

        private:    // Private methods
            // Retrieve the main grid from the shell state into a CLI state.
            PicrossCLIState shellStateToCLIState(PicrossShellState& shellState);
            // Retrieve the main grid from the CLI state into a shell state.
            PicrossShellState CLIStateToShellState(PicrossCLIState& cliState);
            PicrossShell instantiateMicroShell();
    };
} // namespace Picross

#endif//PICROSS_CLI__CLI_MODIFY_GRID_COMMAND_HPP