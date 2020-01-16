#include "../tools/micro_shell/micro_shell_command.hpp"
#include "../tools/micro_shell/micro_shell_codes.hpp"
#include "../tools/cli/cli_input.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/iterable_tools.hpp"
#include "shell_hint_coherence_command.hpp"
#include "picross_shell_state.hpp"

#include <string>

namespace Picross
{
    ShellHintCoherenceCommand::ShellHintCoherenceCommand() :
        MicroShellCommand<PicrossShellState>()
    {

    }

    ShellHintCoherenceCommand::~ShellHintCoherenceCommand()
    {

    }

    int ShellHintCoherenceCommand::processInput(const std::string& command, PicrossShellState& state, CLIStreams& streams)
    {
        std::vector<std::string> tokens = StringTools::tokenizeString(command, ' ', true);

        if (tokens.size() > 1)          // Too many arguments.
        {
            streams.out() << "hintCoherence: no arguments expected." << std::endl;
            return SHELL_COMMAND_BAD_ARGUMENTS;
        }

        // Display whether hints are coherence.
        if (state.workingGrid().hintsAreConsistent())
        {
            streams.out() << "true" << std::endl;
        }
        else
        {
            streams.out() << "false: sum of row hints is " << state.workingGrid().rowHintSum() << ", sum of column hints is " << state.workingGrid().colHintSum() << "." << std::endl;
        }
        
        return SHELL_COMMAND_SUCCESS;
    }

    std::string ShellHintCoherenceCommand::name()
    {
        return "hintCoherence";
    }

    std::string ShellHintCoherenceCommand::description()
    {
        return "Tells whether hints of the grid are consistent";
    }

    std::string ShellHintCoherenceCommand::help()
    {
        std::string s;
        s += "hintCoherence - tells whether the sum of hints is the same for each dimension.\n";
        s += "No arguments.\n";
        return s;
    }
} // namespace Picross
