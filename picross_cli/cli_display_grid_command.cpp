#include "cli_display_grid_command.hpp"

#include <string>
#include "../io/text_grid_formatter.hpp"
#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "picross_cli_state.hpp"

namespace Picross
{
    CLIDisplayGridCommand::CLIDisplayGridCommand() : CLICommand<PicrossCLIState>()
    {

    }

    CLIDisplayGridCommand::~CLIDisplayGridCommand()
    {

    }

    std::string CLIDisplayGridCommand::getTooltip()
    {
        return "Display grid";
    }

    int CLIDisplayGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        TextGridFormatter txt = TextGridFormatter();
        streams.out() << txt.renderGridWithHints(state.grid());

        return CLI_COMMAND_SUCCESS;
    }
} // namespace Picross