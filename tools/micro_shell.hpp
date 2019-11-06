#ifndef TOOLS__MICRO_SHELL_HPP
#define TOOLS__MICRO_SHELL_HPP

#include <string>
#include <deque>
#include <memory>
#include "micro_shell_command.hpp"

class MicroShell
{
    using CommandPtr = std::shared_ptr<MicroShellCommand>;
    using ChainIter = std::deque<CommandPtr>::iterator;

    private:    // Attributes
        std::deque<CommandPtr> _chain;

    public:     // Public methods
        MicroShell();

    // Command manipulation methods
        void clearCommands();
        void addCommand(CommandPtr command, int index = -1);
        void removeCommand(int index);
        void removeCommand(std::string name);

    // Getter methods
        CommandPtr getCommand(int index);
        CommandPtr getCommand(std::string name);

    // Checks
        int indexOf(std::string name);
        bool hasCommand(std::string name);

        int processInput(const std::string& input);
};

#endif//TOOLS__MICRO_SHELL_HPP