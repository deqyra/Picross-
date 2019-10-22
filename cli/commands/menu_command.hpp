#ifndef MENU_COMMAND_HPP
#define MENU_COMMAND_HPP

#include "../cli_command.hpp"

#include <string>

#include "../cli_menu.hpp"
#include "../cli_state.hpp"

namespace Picross
{
    class MenuCommand : public CLICommand
    {
        private:    // Attributes
            CLIMenu _innerMenu;

        public:     // Public methods
            MenuCommand(CLIMenu innerMenu);
            virtual ~MenuCommand();

            virtual std::string getTooltip();
            virtual void run(CLIState& state);
    };
}

#endif//MENU_COMMAND_HPP