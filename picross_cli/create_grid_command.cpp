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
        // Ask for dimensions.
        int height = CLIInput::askForBoundedInput<int>("Enter new grid height: ", 1, GRID_MAX_SIZE, streams);
        int width = CLIInput::askForBoundedInput<int>("Enter new grid width: ", 1, GRID_MAX_SIZE, streams);

        // Ask for hints (horizontal).
        streams.out() << "Input all row hints, one row at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> hHints = handleHintSequenceInput(height, width, "Row", streams);

        // Ask for hints (vertical).
        streams.out() << "Input all column hints, one column at a time, with space-separated values." << std::endl;
        std::vector<std::vector<int>> vHints = handleHintSequenceInput(width, height, "Column", streams);

        try
        {
            // Create the grid.
            state.grid() = Grid(width, height, hHints, vHints);
            streams.out() << "Grid creation successful." << std::endl;
            return COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            // Informative error logging.
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Grid could not be created." << std::endl;

            return COMMAND_FAILURE;
        }
    }

    std::vector<std::vector<int>> CreateGridCommand::handleHintSequenceInput(int sequenceLength, int maxHintSpace, std::string numberedPrefixNoun, CLIStreams& streams)
    {
        // Repeatedly asks for hints that fit in a certain grid.

        std::vector<std::vector<int>> allHints;
        // Looping until all hints are entered...
        for (int i = 0; i < sequenceLength; i++)
        {
            std::vector<int> intHints;
            // Looping while the input is invalid...
            while (true)
            {
                // Prompt the user.
                std::string title = numberedPrefixNoun + " n°" + std::to_string(i) + ": ";
                std::string strHints = CLIInput::askForInput<std::string>(title, streams);

                // Get an int vector out of the input string.
                try
                {
                    intHints = StringTools::stringToIntVector(strHints);
                }
                catch(const std::exception& e)
                {
                    streams.err() << "Input could not be parsed. Please only enter space-separated integers." << std::endl;
                    continue;
                }

                // Check whether the entered hints are all above 0.
                if (!hintsArePositiveNonZero(intHints))
                {
                    streams.err() << "All hints must be above 0. Please try again." << std::endl;
                    continue;
                }

                // Check whether the entered hints fit in the grid.
                if (minimumSpaceFromHints(intHints) > maxHintSpace)
                {
                    streams.err() << "Such hints do not fit in a grid of provided dimensions. Please try again." << std::endl;
                    continue;
                }

                // If all went well, the input was valid: exit the innermost loop.
                break;
            }

            // Register the latest hints.
            allHints.push_back(intHints);
        }

        return allHints;
    }

    bool CreateGridCommand::hintsArePositiveNonZero(std::vector<int>& hints)
    {
        // Return false if any of the provided hints is below 1.
        for (auto it = hints.begin(); it != hints.end(); it++)
        {
            if (*it < 1) return false;
        }
        return true;
    }
}
