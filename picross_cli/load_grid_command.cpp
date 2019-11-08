#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "load_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <string>
#include <iostream>

#include "../io/xml_grid_serializer.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    LoadGridCommand::LoadGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    LoadGridCommand::~LoadGridCommand()
    {
        
    }

    std::string LoadGridCommand::getTooltip()
    {
        return "Load a grid from disk";
    }

    int LoadGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        std::string path = CLIInput::askForInput<std::string>("Enter path to an XML grid file: ", streams);

        XMLGridSerialzer reader;
        try
        {
            Grid loadedGrid = reader.loadGridFromFile(path);
            state.grid() = loadedGrid;
            streams.out() << "Grid successfully loaded." << std::endl;

            return COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Grid could not be loaded." << std::endl;

            return COMMAND_FAILURE;
        }
    }
}