#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "cli_load_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <string>
#include <iostream>

#include "../io/xml_grid_serializer.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    CLILoadGridCommand::CLILoadGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    CLILoadGridCommand::~CLILoadGridCommand()
    {
        
    }

    std::string CLILoadGridCommand::getTooltip()
    {
        return "Load a grid from disk";
    }

    int CLILoadGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        // Get path from user.
        std::string path = CLIInput::askForInput<std::string>("Enter path to an XML grid file: ", streams);

        XMLGridSerialzer reader;
        try
        {
            // Read the file.
            Grid loadedGrid = reader.loadGridFromFile(path);
            state.grid() = loadedGrid;
            streams.out() << "Grid successfully loaded." << std::endl;

            return COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            // Informative error logging.
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Grid could not be loaded." << std::endl;

            return COMMAND_FAILURE;
        }
    }
}