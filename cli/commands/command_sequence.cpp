#include "../cli_command.hpp"
#include "command_sequence.hpp"

#include <vector>
#include <memory>
#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    CommandSequence::CommandSequence(std::vector<std::shared_ptr<CLICommand>> commandList, std::string tooltip) :
        CLICommand(),
        _commandList(commandList),
        _tooltip(tooltip)
    {

    }
    
    CommandSequence::~CommandSequence()
    {
        
    }

    std::string CommandSequence::getTooltip()
    {
        if (!_tooltip.empty())
        {
            return _tooltip;
        }
        return _commandList[0]->getTooltip();
    }

    int CommandSequence::run(CLIState& state)
    {
        for (auto it = _commandList.begin(); it != _commandList.end(); it++)
        {
            int status;
            try
            {
                status = (*it)->run(state);
            }
            catch (const std::exception& e)
            {
                state.err() << "Exception thrown:" << std::endl;
                state.err() << e.what() << std::endl;
                return COMMAND_FAILURE;
            }
            if (status != COMMAND_SUCCESS) return status;
        }

        return COMMAND_SUCCESS;
    }
}