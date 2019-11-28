#include "cli_input.hpp"
#include "../string_tools.hpp"

#include <string>
#include <stdexcept>

//
// Specializations of parseString.
//

template<>
std::string CLIInput::parseString(const std::string& input)
{
    return input;
}

template<>
int CLIInput::parseString(const std::string& input)
{
    if (!StringTools::stringIsNum(input))
    {
        throw std::invalid_argument("parseString<int>: String to parse is not exclusively made of digits and a minus sign, or it is at a wrong position.");
    }
    return std::stoi(input);
}

template<>
bool CLIInput::parseString(const std::string& input)
{
    if (input == "y" || input == "yes" || input == "true")
    {
        return true;
    }

    if (input == "n" || input == "no" || input == "false")
    {
        return false;
    }

    throw std::invalid_argument("parseString<bool>: Invalid string value for expected bool input.");
}

//
// Specializations of typeName.
//

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
