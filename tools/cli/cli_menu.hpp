#ifndef TOOLS__CLI__CLI_MENU_HPP
#define TOOLS__CLI__CLI_MENU_HPP

#include <string>
#include <vector>
#include <memory>

#include "cli_streams.hpp"
#include "cli_command.hpp"
#include "cli_input.hpp"
#include "../../tools/string_tools.hpp"

template<typename CustomState>
class CLIMenu
{
    using CommandPtr = std::shared_ptr<CLICommand<CustomState>>;
    
    private:    // Attributes
        std::vector<CommandPtr> _commands;
        std::string _tooltip;
        std::string _exitName;
        CommandPtr _exitCommand;

    public:     // Public methods
        CLIMenu(std::vector<CommandPtr> commands, std::string tooltip, std::string exitName, CommandPtr exitCommand = nullptr);
        
        std::string getTooltip();
        void show(CustomState& state, CLIStreams& streams = CLIInput::defaultStreams);
        
    private:    // Private methods
        std::string optionString(int n, std::string tooltip);
        std::string allOptionsString();
};

template<typename CustomState>
CLIMenu<CustomState>::CLIMenu(std::vector<CLIMenu<CustomState>::CommandPtr> commands, std::string tooltip, std::string exitName, CLIMenu<CustomState>::CommandPtr exitCommand) :
    _commands(commands),
    _tooltip(tooltip),
    _exitName(exitName),
    _exitCommand(exitCommand)
{

}

template<typename CustomState>
std::string CLIMenu<CustomState>::getTooltip()
{
    return _tooltip;
}

template<typename CustomState>
void CLIMenu<CustomState>::show(CustomState& state, CLIStreams& streams)
{
    while (true)
    {
        streams.out() << '\n';
        streams.out() << getTooltip() << ":" << std::endl;
        streams.out() << allOptionsString();

        int nOptions = _commands.size();
        int input = CLIInput::askForBoundedInput<int>("Please make a choice: ", 0, nOptions, streams);

        if (input == 0)
        {
            if (_exitCommand)
            {
                _exitCommand->run(state, streams);
            }
            break;
        }

        streams.out() << '\n';
        streams.out() << _commands[input - 1]->getTooltip() << ":" << std::endl;
        _commands[input - 1]->run(state, streams);
    }
}

template<typename CustomState>
std::string CLIMenu<CustomState>::optionString(int n, std::string tooltip)
{
    std::string s = std::to_string(n) + ". " + tooltip + "\n";
    return s;
}

template<typename CustomState>
std::string CLIMenu<CustomState>::allOptionsString()
{
    std::string s;

    for (int i = 0; i < _commands.size(); i++)
    {
        s += optionString(i + 1, _commands[i]->getTooltip());
    }
    s += optionString(0, _exitName);

    return s;
}

#endif//TOOLS__CLI__CLI_MENU_HPP