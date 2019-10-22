#include "cli_menu.hpp"
#include "cli_input.hpp"

namespace Picross
{
    CLIMenu::CLIMenu(CLIState& state, std::vector<CLICommand*> commands, std::string tooltip, std::string exitName, CLICommand* exitCommand) :
        _state(state),
        _commands(commands),
        _tooltip(tooltip),
        _exitName(exitName),
        _exitCommand(exitCommand)
    {

    }

    CLIMenu::~CLIMenu()
    {
        for (auto it = _commands.begin(); it != _commands.end(); it++)
        {
            delete (*it);
            (*it) = nullptr;
        }
        
        delete _exitCommand;
        _exitCommand = nullptr;
    }

    std::string CLIMenu::getTooltip()
    {
        return _tooltip;
    }

    void CLIMenu::show()
    {
        while (true)
        {
            showOptions();

            int input = CLIInput::waitForInput<int>(_state);
            if (input < 0 || input > _commands.size())
            {
                _state.err() << "Invalid input, please enter an integer between 0 and " << _commands.size() << "." << std::endl << std::endl; 
                continue;
            }

            if (input == 0)
            {
                if (_exitCommand)
                {
                    _exitCommand->run(_state);
                }
                break;
            }
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