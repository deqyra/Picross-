#include "../cli_command.hpp"
#include "create_grid_command.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "../cli_state.hpp"
#include "../cli_input.hpp"
#include "../../core/utility.hpp"

namespace Picross
{
    CreateGridCommand::CreateGridCommand() :
        CLICommand()
    {

    }

    CreateGridCommand::~CreateGridCommand()
    {
        
    }

    std::string CreateGridCommand::getTooltip()
    {
        return "Create a new grid";
    }

    void CreateGridCommand::run(CLIState& state)
    {
        int height = CLIInput::askForInput<int>("Enter new grid height: ", state);
        int width = CLIInput::askForInput<int>("Enter new grid width: ", state);

        state.out() << "Input all row hints, one row at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> hHints = handleHintSequenceInput(height, "Row", state);

        state.out() << "Input all column hints, one column at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> vHints = handleHintSequenceInput(width, "Column", state);

        state.grid() = Grid(width, height, hHints, vHints);
    }

    std::vector<std::vector<int>> CreateGridCommand::handleHintSequenceInput(int sequenceLength, std::string numberedPrefixNoun, CLIState& state)
    {
        std::vector<std::vector<int>> allHints;
        for (int i = 0; i < sequenceLength; i++)
        {
            std::vector<int> intHints;
            while (true)
            {
                std::stringstream title;
                title << numberedPrefixNoun << " n°" << i << ": ";
                std::string strHints = CLIInput::askForInput<std::string>(title.str(), state);

                try
                {
                    intHints = stringToHintVector(strHints);
                    break;
                }
                catch(const std::exception& e)
                {
                    state.out() << "Input sequence could not be parsed. Please only enter space-separated integers.";
                }                
            }

            allHints.push_back(intHints);
        }

        return allHints;
    }
}
