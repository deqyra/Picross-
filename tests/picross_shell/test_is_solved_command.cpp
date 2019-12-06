#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include <string>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../tools/string_tools.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_is_solved_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellIsSolvedCommand end-to-end", TAGS)
    {
        std::stringstream ss;
        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossShellState state = PicrossShellState();

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_shell/5_10_completed.xml");

        state.mainGrid() = g;
        state.workingGrid() = g;

        ShellIsSolvedCommand command = ShellIsSolvedCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("isSolved azeazeaze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == g);
        }

        SECTION("Valid arguments")
        {
            REQUIRE(command.processInput("isSolved", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == g);

            state.workingGrid().setCellRange(0, 4, 0, 9, CELL_CHECKED);
            REQUIRE(command.processInput("isSolved", state, s) == SHELL_COMMAND_SUCCESS);

            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/is_solved_output.txt");
            REQUIRE(ss.str() == expected);
        }
    }
}
