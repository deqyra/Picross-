#ifndef TOOLS__MICRO_SHELL_HPP
#define TOOLS__MICRO_SHELL_HPP

#include <string>
#include <deque>
#include <memory>
#include "micro_shell_command.hpp"

class MicroShell
{
    private:    // Attributes
        std::deque<std::shared_ptr<MicroShellCommand>> _chain;

    public:     // Public methods
        MicroShell();

        void clearCommands();
        void addCommand(std::shared_ptr<MicroShellCommand> command);
        void removeCommand(std::string name);

        int processInput(const std::string& input);
};

#endif//TOOLS__MICRO_SHELL_HPP