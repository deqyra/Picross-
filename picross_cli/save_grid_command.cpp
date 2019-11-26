#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_streams.hpp"
#include "../tools/cli/cli_input.hpp"
#include "save_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <string>
#include <iostream>

#include "../io/xml_grid_serializer.hpp"

namespace Picross
{
    SaveGridCommand::SaveGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    SaveGridCommand::~SaveGridCommand()
    {
        
    }

    std::string SaveGridCommand::getTooltip()
    {
        return "Save grid to disk";
    }

    int SaveGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        // Get path from user.
        std::string path = CLIInput::askForInput<std::string>("Enter file path to save the XML grid to: ", streams);

        XMLGridSerialzer writer;
        try
        {
            // Save the grid to the file.
            writer.saveGridToFile(state.grid(), path);
            streams.out() << "Grid successfully saved." << std::endl;

            return COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            // Informative error logging.
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Grid could not be saved." << std::endl;

            return COMMAND_FAILURE;
        }
    }
}