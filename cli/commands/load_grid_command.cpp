#include "../cli_command.hpp"
#include "load_grid_command.hpp"

#include <string>
#include <iostream>

#include "../cli_state.hpp"
#include "../cli_input.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../core/grid.hpp"

namespace Picross
{
    LoadGridCommand::LoadGridCommand() :
        CLICommand()
    {

    }

    LoadGridCommand::~LoadGridCommand()
    {
        
    }

    std::string LoadGridCommand::getTooltip()
    {
        return "Load a grid from disk";
    }

    void LoadGridCommand::run(CLIState& state)
    {
        std::string path = CLIInput::askForInput<std::string>("Enter path to an XML grid file: ", state);

        XMLGridSerialzer reader;
        Grid loadedGrid = reader.loadGridFromFile(path);
        state.grid() = loadedGrid;
        state.out() << "Grid successfully loaded." << std::endl;
    }
}