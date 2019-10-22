#include "../cli_command.hpp"
#include "menu_command.hpp"

#include <string>

#include "../cli_state.hpp"

namespace Picross
{
    MenuCommand::MenuCommand(CLIMenu innerMenu) :
        CLICommand(),
        _innerMenu(innerMenu)
    {

    }

    MenuCommand::~MenuCommand()
    {
        
    }

    std::string MenuCommand::getTooltip()
    {
        return _innerMenu.getTooltip();
    }

    int MenuCommand::run(CLIState& state)
    {
        _innerMenu.show();
        return COMMAND_SUCCESS;
    }
}