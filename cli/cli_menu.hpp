#ifndef CLI__CLI_MENU_HPP
#define CLI__CLI_MENU_HPP

#include <string>
#include <vector>
#include <memory>

#include "cli_command.hpp"
#include "cli_state.hpp"
#include "cli_input.hpp"

namespace Picross
{
    class CLIMenu
    {
        private:    // Attributes
            std::vector<std::shared_ptr<CLICommand>> _commands;
            CLIState& _state;
            std::string _tooltip;
            std::string _exitName;
            std::shared_ptr<CLICommand> _exitCommand;

        public:     // Public methods
            CLIMenu(CLIState& state, std::vector<std::shared_ptr<CLICommand>> commands, std::string tooltip, std::string exitName, std::shared_ptr<CLICommand> exitCommand = nullptr);
            
            std::string getTooltip();
            void show();
            
        private:    // Private methods
            void showOption(int n, std::string tooltip);
            void showOptions();
    };
}

#endif//CLI__CLI_MENU_HPP