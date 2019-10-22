#include "cli_input.hpp"

#include <iostream>
#include <string>

namespace Picross
{
    template<>
    std::string CLIInput::parseString(std::string input)
    {
        return input;
    }

    template<>
    int CLIInput::parseString(std::string input)
    {
        return std::stoi(input);
    }

    template<>
    bool CLIInput::parseString(std::string input)
    {
        if (input == "y" || input == "yes" || input == "true")
        {
            return true;
        }

        if (input == "n" || input == "no" || input == "false")
        {
            return false;
        }

        throw std::runtime_error("Invalid string value for expected bool input.");
    }

    template <>
    std::string CLIInput::typeName<std::string>()
    {
        return "string";
    }

    template <>
    std::string CLIInput::typeName<int>()
    {
        return "integer";
    }

    template <>
    std::string CLIInput::typeName<bool>()
    {
        return "boolean (\"y\", \"yes\", \"true\", \"n\", \"no\", \"false\")";
    }
}