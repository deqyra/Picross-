#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_input.hpp"
#include "create_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "../core/grid.hpp"
#include "../core/utility.hpp"
#include "../tools/string_tools.hpp"

namespace Picross
{
    CreateGridCommand::CreateGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    CreateGridCommand::~CreateGridCommand()
    {
        
    }

    std::string CreateGridCommand::getTooltip()
    {
        return "Create a new grid";
    }

    int CreateGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
    {
        int height = CLIInput::askForBoundedInput<int>("Enter new grid height: ", 1, GRID_MAX_SIZE, streams);
        int width = CLIInput::askForBoundedInput<int>("Enter new grid width: ", 1, GRID_MAX_SIZE, streams);

        streams.out() << "Input all row hints, one row at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> hHints = handleHintSequenceInput(height, width, "Row", streams);

        streams.out() << "Input all column hints, one column at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> vHints = handleHintSequenceInput(width, height, "Column", streams);

        state.grid() = Grid(width, height, hHints, vHints);
        streams.out() << "Grid creation successful." << std::endl;
        return COMMAND_SUCCESS;
    }

    std::vector<std::vector<int>> CreateGridCommand::handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIStreams& streams)
    {
        std::vector<std::vector<int>> allHints;
        for (int i = 0; i < sequenceLength; i++)
        {
            std::vector<int> intHints;
            while (true)
            {
                std::string title = numberedPrefixNoun + " n°" + std::to_string(i) + ": ";
                std::string strHints = CLIInput::askForInput<std::string>(title, streams);

                try
                {
                    intHints = StringTools::stringToIntVector(strHints);
                }
                catch(const std::exception& e)
                {
                    streams.err() << "Input sequence could not be parsed. Please only enter space-separated integers." << std::endl;
                    continue;
                }

                if (!hintsAreValid(intHints))
                {
                    streams.err() << "All hints must be above 0. Please try again." << std::endl;
                    continue;
                }

                if (minimumSpaceFromHints(intHints) > maxHintSpace)
                {
                    streams.err() << "Such hints do not fit in a grid of provided dimensions. Please try again." << std::endl;
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
