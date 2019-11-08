#ifndef TOOLS__MICRO_SHELL__MICRO_SHELL_HPP
#define TOOLS__MICRO_SHELL__MICRO_SHELL_HPP

#include <string>
#include <deque>
#include <memory>
#include <vector>
#include <algorithm>
#include "micro_shell_command.hpp"
#include "micro_shell_codes.hpp"
#include "../cli/cli_streams.hpp"
#include "../cli/cli_input.hpp"
#include "../string_tools.hpp"
#include "../exceptions/index_out_of_bounds_error.hpp"

template<typename CustomState>
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

        int processInput(const std::string& input, CustomState& state, CLIStreams& streams = CLIInput::defaultStreams);
};

template<typename CustomState>
MicroShell<CustomState>::MicroShell() :
    _chain()
{

}

template<typename CustomState>
void MicroShell<CustomState>::addCommand(MicroShell<CustomState>::CommandPtr command, int index)
{
    // Do not accept several commands with the same name.
    if (hasCommand(command->name()))
    {
        std::string s = "Command \"" + command->name() + "\" already exists in the shell and cannot be added.";
        throw std::invalid_argument(s.c_str());
    }

    // If index is -1, insert new command at the back.
    if (index == - 1)
    {
        _chain.push_back(command);
    }
    else
    {
        // Else try to insert at given index
        if (_chain.begin() + index >= _chain.end())
        {
            std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot add command.";
            throw IndexOutOfBoundsError(s);
        }
        _chain.insert(_chain.begin() + index, command);
    }
}

template<typename CustomState>
void MicroShell<CustomState>::removeCommand(std::string name)
{
    // Lambda telling whether a given command is of the provided name.
    auto lambda = [name] (CommandPtr c)
    {
        return c->name() == name;
    };

    // Find a command with the same name as provided.
    ChainIter it = std::find_if(_chain.begin(), _chain.end(), lambda);

    // Throw if not found.
    if (it == _chain.end())
    {
        std::string s = "Command \"" + name + "\" is not part of the shell and cannot be removed.";
        throw std::invalid_argument(s.c_str());
    }

    _chain.erase(it);
}

template<typename CustomState>
void MicroShell<CustomState>::removeCommand(int index)
{
    // Try to erase the command at provided index.
    if (_chain.begin() + index >= _chain.end())
    {
        std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot remove command.";
        throw IndexOutOfBoundsError(s);
    }

    _chain.erase(_chain.begin() + index);
}

template<typename CustomState>
void MicroShell<CustomState>::clearCommands()
{
    _chain.clear();
}

template<typename CustomState>
int MicroShell<CustomState>::indexOf(std::string name)
{
    // Lambda telling whether a given command is of the provided name.
    auto lambda = [name] (CommandPtr c)
    {
        return c->name() == name;
    };

    // Find a command with given name.
    ChainIter it = std::find_if(_chain.begin(), _chain.end(), lambda);

    // Calculate and return index, or -1 if not found.
    return it != _chain.end() ? (it - _chain.begin()) : -1;
}

template<typename CustomState>
bool MicroShell<CustomState>::hasCommand(std::string name)
{
    return indexOf(name) != -1;
}

template<typename CustomState>
MicroShell<CustomState>::CommandPtr MicroShell<CustomState>::getCommand(int index)
{
    // Try to erase the command at provided index.
    if (_chain.begin() + index >= _chain.end())
    {
        std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot fetch command.";
        throw IndexOutOfBoundsError(s);
    }

    return _chain.at(index);
}

template<typename CustomState>
MicroShell<CustomState>::CommandPtr MicroShell<CustomState>::getCommand(std::string name)
{
    int index = indexOf(name);
    if (index == -1)
    {
        std::string s = "Command with name " + name + " is not part of the shell and cannot be fetched.";
        throw IndexOutOfBoundsError(s);
    }

    return getCommand(index);
}

template<typename CustomState>
int MicroShell<CustomState>::processInput(const std::string& input, CustomState& state, CLIStreams& streams)
{
    std::vector<std::string> tokens = StringTools::tokenizeString(input, ' ', true);
    int index = indexOf(tokens[0]);
    if (index == - 1)
    {
        return SHELL_COMMAND_NOT_FOUND;
    }

    return _chain[index]->processInput(input);
}


#endif//TOOLS__MICRO_SHELL__MICRO_SHELL_HPP