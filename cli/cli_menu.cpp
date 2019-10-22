#include "cli_menu.hpp"

#include <string>
#include <vector>
#include <memory>

#include "cli_state.hpp"
#include "cli_input.hpp"

namespace Picross
{
    CLIMenu::CLIMenu(CLIState& state, std::vector<std::shared_ptr<CLICommand>> commands, std::string tooltip, std::string exitName, std::shared_ptr<CLICommand> exitCommand) :
        _state(state),
        _commands(commands),
        _tooltip(tooltip),
        _exitName(exitName),
        _exitCommand(exitCommand)
    {

    }

    std::string CLIMenu::getTooltip()
    {
        return _tooltip;
    }

    void CLIMenu::show()
    {
        while (true)
        {
        	_state.out() << getTooltip() << ":" << std::endl;
            showOptions();

            int nOptions = _commands.size();
            int input = CLIInput::askForBoundedInput<int>("Please make a choice: ", _state, 0, nOptions);

            if (input == 0)
            {
                if (_exitCommand)
                {
                    _exitCommand->run(_state);
                }
                break;
            }

            _state.out() << std::endl;
            _state.out() << _commands[input - 1]->getTooltip() << ":" << std::endl;
            _commands[input - 1]->run(_state);
        }
    }

    void CLIMenu::showOption(int n, std::string tooltip)
    {
        _state.out() << n << ". " << tooltip << std::endl;
    }

    void CLIMenu::showOptions()
    {
        for (int i = 0; i < _commands.size(); i++)
        {
            showOption(i + 1, _commands[i]->getTooltip());
        }
        showOption(0, _exitName);
    }
}