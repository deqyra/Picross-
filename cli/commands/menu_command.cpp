#include "menu_command.hpp"

namespace Picross
{
    MenuCommand::MenuCommand(CLIMenu innerMenu) :
        CLICommand(),
        _innerMenu(innerMenu)
    {

    }

    std::string MenuCommand::getTooltip()
    {
        return _innerMenu.getTooltip();
    }

    void MenuCommand::run(CLIState& state)
    {
        _innerMenu.show();
    }
}