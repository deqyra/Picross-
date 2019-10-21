#ifndef MENU_COMMAND_HPP
#define MENU_COMMAND_HPP

#include "../cli_command.hpp"
#include "../cli_menu.hpp"
#include "../cli_state.hpp"

namespace Picross
{
    class MenuCommand : public CLICommand
    {
        private:
            CLIMenu _innerMenu;

        public:
            MenuCommand(CLIMenu innerMenu);
            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//MENU_COMMAND_HPP