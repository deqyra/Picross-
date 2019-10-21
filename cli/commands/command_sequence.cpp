#include "command_sequence.hpp"

namespace Picross
{
    CommandSequence::CommandSequence(std::vector<CLICommand*> commandList, std::string tooltip) :
        CLICommand(),
        _commandList(commandList),
        _tooltip(tooltip)
    {
    }
    
    CommandSequence::~CommandSequence()
    {
        for (auto it = _commandList.begin(); it != _commandList.end(); it++)
        {
            delete (*it);
            (*it) = nullptr;
        }
    }

    std::string CommandSequence::getTooltip()
    {
        if (!_tooltip.empty())
        {
            return _tooltip;
        }
        return _commandList[0]->getTooltip();
    }

    void CommandSequence::run(CLIState& state)
    {
        for (auto it = _commandList.begin(); it != _commandList.end(); it++)
        {
            (*it)->run(state);
        }
    }
}