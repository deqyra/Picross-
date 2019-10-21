#ifndef CLI_INPUT
#define CLI_INPUT

#include <iostream>
#include <string>

#include "cli_state.hpp"

namespace Picross
{
    class CLIInput
    {
        public:
            template<typename T>
            static T askForInput(std::string title, CLIState& state);

            template<typename T>
            static T waitForInput(CLIState& state);

            template<typename T>
            static T parseString(std::string input);

            template<typename T>
            static std::string typeName();
    };

    template<typename T>
    T CLIInput::askForInput(std::string title, CLIState& state)
    {
        state.out() << title;
        return waitForInput<T>(state);
    }

    template<typename T>
    T CLIInput::waitForInput(CLIState& state)
    {
        std::string input;

        while (true)
        {
            try
            {
                state.in() >> input;
                return parseString<T>(input);
            }
            catch(const std::invalid_argument& e)
            {
                state.err() << "Invalid input, please enter a " << typeName<T>() << "." << std::endl;
            }            
        }
    }

    template<>
    std::string CLIInput::parseString(std::string input);
    
    template<>
    int CLIInput::parseString(std::string input);

    template <>
    std::string CLIInput::typeName<int>();

    template <>
    std::string CLIInput::typeName<std::string>();
}

#endif//CLI_INPUT