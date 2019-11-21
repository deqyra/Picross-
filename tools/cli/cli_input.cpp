#include "cli_input.hpp"
#include "../string_tools.hpp"

#include <string>
#include <stdexcept>

template<>
std::string CLIInput::parseString(std::string input)
{
    return input;
}

template<>
int CLIInput::parseString(std::string input)
{
    if (!StringTools::stringIsNum(input))
    {
        throw std::invalid_argument("String to parse is not exclusively made of digits and a minus sign, or it is at a wrong position.");
    }
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

    throw std::invalid_argument("Invalid string value for expected bool input.");
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
