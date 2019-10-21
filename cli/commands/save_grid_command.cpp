#include "save_grid_command.hpp"
#include "../cli_input.hpp"
#include "../../io/xml_grid_serializer.hpp"

namespace Picross
{
    SaveGridCommand::SaveGridCommand() :
        CLICommand()
    {

    }

    std::string SaveGridCommand::getTooltip()
    {
        return "Load a grid from disk";
    }

    void SaveGridCommand::run(CLIState& state)
    {
        std::string path = CLIInput::askForInput<std::string>("Enter file path to save the XML grid to: ", state);

        XMLGridSerialzer writer;
        writer.saveGridToFile(state.grid(), path);
        state.out() << "Grid successfully saved." << std::endl;
    }
}