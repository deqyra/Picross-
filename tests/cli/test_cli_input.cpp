#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "../../cli/cli_state.hpp"
#include "../../cli/cli_input.hpp"

#define TAGS "[cli][cli_input]"

namespace Picross
{
    SCENARIO("CLI input functions work", TAGS)
    {
        GIVEN("An int input file")
        {
            std::ifstream f = std::ifstream("resources/tests/cli/int_input.txt", std::ios::in);
            REQUIRE(f);
            std::stringstream ss;
            CLIState s = CLIState(f, ss, ss);

            THEN("Related input functions work")
            {
                REQUIRE(CLIInput::askForInput<int>("input1: ", s) == 30);
                REQUIRE(CLIInput::askForInput<int>("input2: ", s) == -20);
                REQUIRE(CLIInput::askForMinInput<int>("minInput: ", s, 40) == 40);
                REQUIRE(CLIInput::askForMaxInput<int>("maxInput: ", s, 40) == 40);
                REQUIRE(CLIInput::askForBoundedInput<int>("boundedInput: ", s, 15, 25) == 20);

                std::string expected =  "input1: Invalid input, please enter a integer.\n"
                                        "input1: input2: minInput: Invalid input, please enter a value that is at least 40.\n"
                                        "minInput: maxInput: Invalid input, please enter a value that is at most 40.\n"
                                        "maxInput: boundedInput: Invalid input, please enter a value between 15 and 25.\n"
                                        "boundedInput: Invalid input, please enter a value between 15 and 25.\n"
                                        "boundedInput: ";

                REQUIRE(ss.str() == expected);
            }
        }

        AND_GIVEN("A bool input file")
        {
            std::ifstream f = std::ifstream("resources/tests/cli/bool_input.txt", std::ios::in);
            REQUIRE(f);
            std::stringstream ss;
            CLIState s = CLIState(f, ss, ss);

            THEN("Related input functions work")
            {
                REQUIRE(CLIInput::askForInput<bool>("inputY: ", s));
                REQUIRE(CLIInput::askForInput<bool>("inputYes: ", s));
                REQUIRE(CLIInput::askForInput<bool>("inputTrue: ", s));
                REQUIRE_FALSE(CLIInput::askForInput<bool>("inputN: ", s));
                REQUIRE_FALSE(CLIInput::askForInput<bool>("inputNo: ", s));
                REQUIRE_FALSE(CLIInput::askForInput<bool>("inputFalse: ", s));

                std::string expected =  "inputY: Invalid input, please enter a boolean (\"y\", \"yes\", \"true\", \"n\", \"no\", \"false\").\n"
                                        "inputY: inputYes: inputTrue: inputN: inputNo: inputFalse: ";

                REQUIRE(ss.str() == expected);
            }
        }
    }
}
