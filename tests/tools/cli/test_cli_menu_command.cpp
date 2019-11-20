#include "../../../lib/catch2/catch2.hpp"

#include "cli_test_classes.hpp"
#include "../../../tools/cli/cli_menu.hpp"
#include "../../../tools/cli/cli_streams.hpp"

#include <string>
#include <fstream>
#include <sstream>

#define TAGS "[cli][cli_command]"

TEST_CASE("MenuCommand end-to-end", TAGS)
{
    std::ifstream f = std::ifstream("resources/tests/tools/cli/menu_input.txt", std::ios::in);
    REQUIRE(f);
    std::stringstream ss;

    CLIStreams s = CLIStreams(f, ss, ss);
    TestCLIState state = TestCLIState();

    TestMenu menu = makeBasicTestMenu();
    TestMenuCommand menuCommand = TestMenuCommand(menu);
    menuCommand.run(state, s);

    std::string expected =  "\n"
                            "Test menu:\n"
                            "1. TestCommand1\n"
                            "2. TestCommand2\n"
                            "0. Exit\n"
                            "Please make a choice: \n"
                            "TestCommand1:\n"
                            "TestCommand1 was run.\n"
                            "\n"
                            "Test menu:\n"
                            "1. TestCommand1\n"
                            "2. TestCommand2\n"
                            "0. Exit\n"
                            "Please make a choice: \n"
                            "TestCommand2:\n"
                            "TestCommand2 was run.\n"
                            "\n"
                            "Test menu:\n"
                            "1. TestCommand1\n"
                            "2. TestCommand2\n"
                            "0. Exit\n"
                            "Please make a choice: Successful exit.\n";

    REQUIRE(ss.str() == expected);

    f.close();
}
