#include "micro_shell.hpp"
#include "micro_shell_codes.hpp"

#include <string>
#include <algorithm>
#include "string_tools.hpp"
#include "exceptions/index_out_of_bounds_error.hpp"
#include "exceptions/shell_command_not_found_error.hpp"

MicroShell::MicroShell() :
    _chain()
{

}

void MicroShell::addCommand(CommandPtr command, int index)
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

void MicroShell::removeCommand(std::string name)
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

void MicroShell::removeCommand(int index)
{
    // Try to erase the command at provided index.
    if (_chain.begin() + index >= _chain.end())
    {
        std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot remove command.";
        throw IndexOutOfBoundsError(s);
    }

    _chain.erase(_chain.begin() + index);
}

void MicroShell::clearCommands()
{
    _chain.clear();
}

int MicroShell::indexOf(std::string name)
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

bool MicroShell::hasCommand(std::string name)
{
    return indexOf(name) != -1;
}

MicroShell::CommandPtr MicroShell::getCommand(int index)
{
    // Try to erase the command at provided index.
    if (_chain.begin() + index >= _chain.end())
    {
        std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot fetch command.";
        throw IndexOutOfBoundsError(s);
    }

    return _chain.at(index);
}

MicroShell::CommandPtr MicroShell::getCommand(std::string name)
{
    int index = indexOf(name);
    if (index == -1)
    {
        std::string s = "Command with name " + name + " is not part of the shell and cannot be fetched.";
        throw IndexOutOfBoundsError(s);
    }

    return getCommand(index);
}

int MicroShell::processInput(const std::string& input)
{
    std::vector<std::string> tokens = StringTools::tokenizeString(input, ' ', true);
    int index = indexOf(tokens[0]);
    if (index == - 1)
    {
        return SHELL_COMMAND_NOT_FOUND;
    }

    return _chain[index]->processInput(input);
}
