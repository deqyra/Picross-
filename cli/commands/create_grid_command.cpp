#include "../cli_command.hpp"
#include "create_grid_command.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#include "../cli_state.hpp"
#include "../cli_input.hpp"
#include "../../core/utility.hpp"
#include "../../core/grid.hpp"

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

    int CreateGridCommand::run(CLIState& state)
    {
        int height = CLIInput::askForBoundedInput<int>("Enter new grid height: ", state, 1, GRID_MAX_SIZE);
        int width = CLIInput::askForBoundedInput<int>("Enter new grid width: ", state, 1, GRID_MAX_SIZE);

        state.out() << "Input all row hints, one row at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> hHints = handleHintSequenceInput(height, width, "Row", state);

        state.out() << "Input all column hints, one column at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> vHints = handleHintSequenceInput(width, height, "Column", state);

        state.grid() = Grid(width, height, hHints, vHints);
        state.out() << "Grid creation successful." << std::endl;
        return COMMAND_SUCCESS;
    }

    std::vector<std::vector<int>> CreateGridCommand::handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIState& state)
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
                }
                catch(const std::exception& e)
                {
                    state.err() << "Input sequence could not be parsed. Please only enter space-separated integers." << std::endl;
                    continue;
                }

                if (!hintsAreValid(intHints))
                {
                    state.err() << "All hints must be above 0. Please try again." << std::endl;
                    continue;
                }

                if (minimumSpaceFromHints(intHints) > maxHintSpace)
                {
                    state.err() << "Such hints do not fit in a grid of provided dimensions. Please try again." << std::endl;
                    continue;
                }

                break;
            }

            allHints.push_back(intHints);
        }

        return allHints;
    }

    bool CreateGridCommand::hintsAreValid(std::vector<int>& hints)
    {
        for (auto it = hints.begin(); it != hints.end(); it++)
        {
            if (*it < 1) return false;
        }
        return true;
    }
}
