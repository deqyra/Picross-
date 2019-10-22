#ifndef CLI_INPUT
#define CLI_INPUT

#include <iostream>
#include <string>

#include "cli_state.hpp"

namespace Picross
{
    class CLIInput
    {
        public:     // Public methods
            template<typename T>
            static T askForInput(std::string title, CLIState& state);

            template<typename T>
            static T askForMinInput(std::string title, CLIState& state, T min);

            template<typename T>
            static T askForMaxInput(std::string title, CLIState& state, T max);

            template<typename T>
            static T askForBoundedInput(std::string title, CLIState& state, T min, T max);

            template<typename T>
            static T parseString(std::string input);

            template<typename T>
            static std::string typeName();

        private:    // Private methods
            template<typename T>
            static T waitForInput(CLIState& state);
    };

    template<typename T>
    T CLIInput::askForInput(std::string title, CLIState& state)
    {
        while (true)
        {
            try
            {
                state.out() << title;
                return waitForInput<T>(state);
            }
            catch(const std::invalid_argument& e)
            {
                state.err() << "Invalid input, please enter a " << typeName<T>() << "." << std::endl;
            }            
        }
    }

    template<typename T>
    T CLIInput::askForMinInput(std::string title, CLIState& state, T min)
    {
        while (true)
        {
            T input = askForInput<T>(title, state);

            if (input >= min)
            {
                return input;
            }

            state.err() << "Invalid input, please enter a value greater than or equal to" << min << "." << std::endl;
        }
    }

    template<typename T>
    T CLIInput::askForMaxInput(std::string title, CLIState& state, T max)
    {
        while (true)
        {
            T input = askForInput<T>(title, state);

            if (input <= max)
            {
                return input;
            }

            state.err() << "Invalid input, please enter a value less than or equal to " << max << "." << std::endl;
        }
    }

    template<typename T>
    T CLIInput::askForBoundedInput(std::string title, CLIState& state, T min, T max)
    {
        while (true)
        {
            T input = askForInput<T>(title, state);

            if (input >= min && input <= max)
            {
                return input;
            }

            state.err() << "Invalid input, please enter a value between " << min << " and " << max << "." << std::endl;
        }
    }

    template<typename T>
    T CLIInput::waitForInput(CLIState& state)
    {
        std::string input;
        std::getline(state.in(), input);
        return parseString<T>(input);
    }

    template<>
    std::string CLIInput::parseString(std::string input);
    
    template<>
    int CLIInput::parseString(std::string input);
    
    template<>
    bool CLIInput::parseString(std::string input);

    template <>
    std::string CLIInput::typeName<std::string>();

    template <>
    std::string CLIInput::typeName<int>();

    template <>
    std::string CLIInput::typeName<bool>();
}

#endif//CLI_INPUT