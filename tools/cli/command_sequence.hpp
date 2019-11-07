#ifndef TOOLS__CLI__COMMAND_SEQUENCE_HPP
#define TOOLS__CLI__COMMAND_SEQUENCE_HPP

#include <vector>
#include <string>
#include <memory>

#include "cli_command.hpp"
#include "cli_input.hpp"

template <typename CustomState>
class CommandSequence : public CLICommand<CustomState>
{
    using CommandPtr = std::shared_ptr<CLICommand<CustomState>>;

    private:    // Attributes
        std::vector<CommandPtr> _commandList;
        std::string _tooltip;

    public:     // Public methods
        CommandSequence(std::vector<CommandPtr> commandList, std::string tooltip = "");
        virtual ~CommandSequence();
        
        virtual std::string getTooltip();
        virtual int run(CustomState& state, CLIStreams& streams = CLIInput::defaultStreams);
};

template <typename CustomState>
CommandSequence<CustomState>::CommandSequence(std::vector<CommandSequence<CustomState>::CommandPtr> commandList, std::string tooltip) :
    CLICommand<CustomState>(),
    _commandList(commandList),
    _tooltip(tooltip)
{

}

template <typename CustomState>
CommandSequence<CustomState>::~CommandSequence()
{
    
}

template <typename CustomState>
std::string CommandSequence<CustomState>::getTooltip()
{
    if (!_tooltip.empty())
    {
        return _tooltip;
    }
    return _commandList[0]->getTooltip();
}

template <typename CustomState>
int CommandSequence<CustomState>::run(CustomState& state, CLIStreams& streams)
{
    for (auto it = _commandList.begin(); it != _commandList.end(); it++)
    {
        int status;
        try
        {
            status = (*it)->run(state, streams);
        }
        catch (const std::exception& e)
        {
            streams.err() << "Exception thrown:\n";
            streams.err() << e.what() << std::endl;
            return COMMAND_FAILURE;
        }
        if (status != COMMAND_SUCCESS) return status;
    }

    return COMMAND_SUCCESS;
}

#endif//TOOLS__CLI__COMMAND_SEQUENCE_HPP