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
    using PicrossShellCommand = MicroShellCommand<CustomState>;
    using CommandPtr = std::shared_ptr<PicrossShellCommand>;
    using ChainIter = std::deque<CommandPtr>::iterator;

    private:    // Attributes
        std::deque<CommandPtr> _chain;
        CommandPtr _exitCommand;

    public:     // Public methods
        MicroShell();

    // Command manipulation methods
        void clearCommands();
        void addCommand(CommandPtr command, int index = -1);
        void removeCommand(int index);
        void removeCommand(std::string name);
        void setExitCommand(CommandPtr command);

    // Getter methods
        CommandPtr getCommand(int index);
        CommandPtr getCommand(std::string name);
        CommandPtr getExitCommand();

    // Checks
        int indexOf(std::string name);
        bool hasCommand(std::string name);

        int run(CustomState& state, CLIStreams& streams = CLIInput::defaultStreams);

    private:    // Private methods
        int processInput(const std::string& input, CustomState& state, CLIStreams& streams);
        std::string globalHelpString();
        std::string commandHelpString(std::string commandName);
        void handleExit(std::string command, CustomState& state, CLIStreams& streams);
};

template<typename CustomState>
MicroShell<CustomState>::MicroShell() :
    _chain(),
    _exitCommand(nullptr)
{

}

template<typename CustomState>
void MicroShell<CustomState>::addCommand(MicroShell<CustomState>::CommandPtr command, int index)
{
    // Do not accept several commands with the same name, nor any command named 'exit' or 'help'.
    if (hasCommand(command->name()) || command->name() == EXIT_KEYWORD || command->name() == HELP_KEYWORD)
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
    // Command exists if a valid index is found.
    return indexOf(name) != -1;
}

template<typename CustomState>
void MicroShell<CustomState>::setExitCommand(MicroShell<CustomState>::CommandPtr command)
{
    _exitCommand = command;
}

template<typename CustomState>
MicroShell<CustomState>::CommandPtr MicroShell<CustomState>::getCommand(int index)
{
    // Try to fetch the command at provided index.
    if (index >= _chain.size())
    {
        std::string s = "Index " + std::to_string(index) + " is out of bounds, cannot fetch command.";
        throw IndexOutOfBoundsError(s);
    }

    return _chain.at(index);
}

template<typename CustomState>
MicroShell<CustomState>::CommandPtr MicroShell<CustomState>::getCommand(std::string name)
{
    // Try to fetch the command at provided index.
    int index = indexOf(name);
    if (index == -1)
    {
        std::string s = "Command with name " + name + " is not part of the shell and cannot be fetched.";
        throw IndexOutOfBoundsError(s);
    }

    return getCommand(index);
}

template<typename CustomState>
MicroShell<CustomState>::CommandPtr MicroShell<CustomState>::getExitCommand()
{
    return _exitCommand;
}

template<typename CustomState>
int MicroShell<CustomState>::run(CustomState& state, CLIStreams& streams = CLIInput::defaultStreams)
{
    // Run the shell: prompt the user repeatedly and interpret the commands that were entered.
    int shellCode;
    do
    {
        std::string command = CLIInput::askForInput<std::string>("$ ", streams);
        shellCode = shell.processInput(command, shellState, streams);
    } while (shellCode != SHELL_EXIT);      // Exit when the proper signal is returned.

    return SHELL_EXIT;
}

template<typename CustomState>
int MicroShell<CustomState>::processInput(const std::string& input, CustomState& state, CLIStreams& streams)
{
    // Tokenise the string on spaces to extract the command and its arguments.
    std::vector<std::string> tokens = StringTools::tokenizeString(input, ' ', true);
    if (!tokens.size()) return SHELL_COMMAND_NOT_FOUND;

    // If help was requested, respond accordingly.
    if (tokens[0] == HELP_KEYWORD)
    {
        // If more than one token was extracted, the second one is
        // probably the command the user want to get help about.
        if (tokens.size() >= 1)
        {
            streams.out() << commandHelpString(tokens[1]) << std::endl;;
        }
        // Otherwise, display an informative list of commands.
        else
        {
            streams.out() << globalHelpString() << std::endl;
        }

        return SHELL_COMMAND_SUCCESS;
    }

    // If exit was entered, handle the exit procedure.
    if (tokens[0] == EXIT_KEYWORD)
    {
        return handleExit(input, state, streams);
    }

    // Otherwise, search for a command sharing the name of the first token...
    int index = indexOf(tokens[0]);
    if (index == - 1)
    {
        streams.out() << tokens[0] << ": command not found.\n";
        return SHELL_COMMAND_NOT_FOUND;
    }

    // ...and run that command.
    return _chain[index]->processInput(input, state, streams);
}

template<typename CustomState>
std::string MicroShell<CustomState>::globalHelpString()
{
    // List available commands.
    std::string s = "Available commands:\n";
    for (auto it = _chain.begin(); it != _chain.end(); it++)
    {
        s += "  - " + (*it)->name() + ": " + (*it)->description() + "\n";
    }
    s += "Type 'help <command>' to get help about one command.\n"

    return s;
}

template<typename CustomState>
std::string MicroShell<CustomState>::commandHelpString(std::string commandName)
{
    // Print the help message of the command which was requested.
    std::string s;
    int index = indexOf(commandName);

    if (index == -1)
    {
        s = "Command '" + commandName + "' not found, help cannot be displayed.\n";
        return s;
    }

    s = "'" + commandName + "' help:\n";
    s += _chain[index]->help();

    return s;
}

template<typename CustomState>
void MicroShell<CustomState>::handleExit(std::string command, CustomState& state, CLIStreams& streams)
{
    // If not exit command was provided, just exit immediately.
    if (!_exitCommand)
    {
        return SHELL_EXIT;
    }
    // Otherwise run the provided exit command.
    return _exitCommand->processInput(command, state, streams)
}

#endif//TOOLS__MICRO_SHELL__MICRO_SHELL_HPP