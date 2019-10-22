#include "../cli_command.hpp"
#include "save_grid_command.hpp"

#include <string>
#include <iostream>

#include "../cli_input.hpp"
#include "../../io/xml_grid_serializer.hpp"

namespace Picross
{
    SaveGridCommand::SaveGridCommand() :
        CLICommand()
    {

    }

    SaveGridCommand::~SaveGridCommand()
    {
        
    }

    std::string SaveGridCommand::getTooltip()
    {
        return "Save grid to disk";
    }

    int SaveGridCommand::run(CLIState& state)
    {
        std::string path = CLIInput::askForInput<std::string>("Enter file path to save the XML grid to: ", state);

        XMLGridSerialzer writer;
        try
        {
            writer.saveGridToFile(state.grid(), path);
            state.out() << "Grid successfully saved." << std::endl;

            return COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            state.err() << "Exception thrown:" << std::endl;
            state.err() << e.what() << std::endl;
            state.out() << "Grid could not saved." << std::endl;

            return COMMAND_FAILURE;
        }
    }
}