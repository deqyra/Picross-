#ifndef CLI__CLI_INPUT
#define CLI__CLI_INPUT

#include <iostream>
#include <string>

#include "cli_state.hpp"
#include "../tools/string_tools.hpp"

namespace Picross
{
    class CLIInput
    {
        public:     // Public methods
            // Prompts the user for input using the provided string as title. Loops until input is successfully parsed.
            template<typename T>
            static T askForInput(std::string title, CLIState& state);

            // Repeatedly asks for input until entered value is at least the provided minimum value. Should be used on orderable types when it makes sense.
            template<typename T>
            static T askForMinInput(std::string title, CLIState& state, T min);

            // Repeatedly asks for input until entered value is at most the provided maximum value. Should be used on orderable types when it makes sense.
            template<typename T>
            static T askForMaxInput(std::string title, CLIState& state, T max);

            // Repeatedly asks for input until entered value is within provided bounds. Should be used on orderable types when it makes sense.
            template<typename T>
            static T askForBoundedInput(std::string title, CLIState& state, T min, T max);

            // Parses a string to find a value of the template parameter. Needs an explicit specialized definition for each type.
            template<typename T>
            static T parseString(std::string input);

            // Returns a descriptive string of the template parameter. Needs an explicit specialized definition for each type.
            template<typename T>
            static std::string typeName();

        private:    // Private methods
            template<typename T>
            static T waitForInput(CLIState& state);
    };

    template<typename T>
    T CLIInput::askForInput(std::string title, CLIState& state)
    {
        // Prompt the user for a value until a valid one is entered.
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
        // Ask for input until the entered value is valid and at least min.
        while (true)
        {
            T input = askForInput<T>(title, state);

            if (input >= min)
            {
                return input;
            }

            state.err() << "Invalid input, please enter a value that is at least " << min << "." << std::endl;
        }
    }

    template<typename T>
    T CLIInput::askForMaxInput(std::string title, CLIState& state, T max)
    {
        // Ask for input until the entered value is valid and at most max.
        while (true)
        {
            T input = askForInput<T>(title, state);

            if (input <= max)
            {
                return input;
            }

            state.err() << "Invalid input, please enter a value that is at most " << max << "." << std::endl;
        }
    }

    template<typename T>
    T CLIInput::askForBoundedInput(std::string title, CLIState& state, T min, T max)
    {
        // Ask for input until the entered value is between min and max.
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
        // Take care of terminal-induced bullcrap.
        StringTools::popCR(input);        
        return parseString<T>(input);
    }

    //
    // Specializations of parseString.
    //

    template<>
    std::string CLIInput::parseString(std::string input);
    
    template<>
    int CLIInput::parseString(std::string input);
    
    template<>
    bool CLIInput::parseString(std::string input);

    //
    // Specializations of typeName.
    //

    template <>
    std::string CLIInput::typeName<std::string>();

    template <>
    std::string CLIInput::typeName<int>();

    template <>
    std::string CLIInput::typeName<bool>();
}

#endif//CLI__CLI_INPUT