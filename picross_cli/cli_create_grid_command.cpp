#include "../tools/cli/cli_command.hpp"
#include "../tools/cli/cli_input.hpp"
#include "cli_create_grid_command.hpp"
#include "picross_cli_state.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "../core/grid.hpp"
#include "../core/utility.hpp"
#include "../tools/string_tools.hpp"
#include "../tools/lambda_maker.hpp"

namespace Picross
{
    CLICreateGridCommand::CLICreateGridCommand() :
        CLICommand<PicrossCLIState>()
    {

    }

    CLICreateGridCommand::~CLICreateGridCommand()
    {
        
    }

    std::string CLICreateGridCommand::getTooltip()
    {
        return "Create a new grid";
    }

    int CLICreateGridCommand::run(PicrossCLIState& state, CLIStreams& streams)
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
            return CLI_COMMAND_SUCCESS;
        }
        catch(const std::exception& e)
        {
            // Informative error logging.
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << '\n';
            streams.out() << "Grid could not be created." << std::endl;

            return CLI_COMMAND_FAILURE;
        }
    }

    std::vector<std::vector<int>> CLICreateGridCommand::handleHintSequenceInput(int sequenceLength, int maxHintSpace, const std::string& numberedPrefixNoun, CLIStreams& streams)
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
                    streams.out() << "Input could not be parsed. Please only enter space-separated integers." << std::endl;
                    continue;
                }

                // Check whether the entered hints are all above 0.
                if (!std::all_of(intHints.begin(), intHints.end(), LambdaMaker::greater_than(0)))
                {
                    streams.out() << "All hints must be above 0. Please try again." << std::endl;
                    continue;
                }

                // Check whether the entered hints fit in the grid.
                if (minimumSpaceFromHints(intHints) > maxHintSpace)
                {
                    streams.out() << "Such hints do not fit in a grid of provided dimensions. Please try again." << std::endl;
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
} // namespace Picross
