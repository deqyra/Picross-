#ifndef TOOLS__MICRO_SHELL_COMMAND_HPP
#define TOOLS__MICRO_SHELL_COMMAND_HPP

#include <string>

class MicroShellCommand
{
    private:    // Attributes
    public:     // Public methods
        MicroShellCommand();
        virtual ~MicroShellCommand();

        virtual int processInput(std::string command) = 0;
        virtual std::string name() = 0;
        virtual std::string description() = 0;
        virtual std::string help() = 0;
};

#endif//TOOLS__MICRO_SHELL_COMMAND_HPP