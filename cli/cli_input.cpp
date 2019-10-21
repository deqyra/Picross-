#include "cli_input.hpp"

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

    template <>
    std::string CLIInput::typeName<int>()
    {
        return "integer";
    }

    template <>
    std::string CLIInput::typeName<std::string>()
    {
        return "string";
    }
}