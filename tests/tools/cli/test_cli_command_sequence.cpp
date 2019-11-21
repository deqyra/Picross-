#include "../../../lib/catch2/catch2.hpp"

#include "cli_test_classes.hpp"
#include "../../../tools/cli/cli_menu.hpp"
#include "../../../tools/cli/cli_streams.hpp"
#include "../../../tools/string_tools.hpp"

#include <string>
#include <fstream>
#include <sstream>

#define TAGS "[cli][cli_command]"

TEST_CASE("CommandSequence end-to-end", TAGS)
{
    std::ifstream f = std::ifstream("resources/tests/tools/cli/menu_input.txt", std::ios::in);
    REQUIRE(f);
    std::stringstream ss;

    CLIStreams s = CLIStreams(f, ss, ss);
    TestCLIState state = TestCLIState();

    TestCommandSequence commandSequence = makeBasicTestCommandSequence();

    commandSequence.run(state, s);

    std::string expected = StringTools::readFileIntoString("resources/tests/tools/cli/menu_output.txt");
    expected += "TestCommand1 was run.\n"
                "TestCommand2 was run.\n";

    std::string str = ss.str();

    REQUIRE(ss.str() == expected);

    f.close();
}
