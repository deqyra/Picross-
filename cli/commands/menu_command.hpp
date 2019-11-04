#ifndef CLI__COMMANDS__MENU_COMMAND_HPP
#define CLI__COMMANDS__MENU_COMMAND_HPP

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
            virtual int run(CLIState& state);
    };
}

#endif//CLI__COMMANDS__MENU_COMMAND_HPP