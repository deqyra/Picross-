#include "load_grid_command.hpp"
#include "../cli_input.hpp"
#include "../../io/xml_grid_serializer.hpp"

namespace Picross
{
    LoadGridCommand::LoadGridCommand() :
        CLICommand()
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