#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_display_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../tools/string_tools.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellDisplayCommand end-to-end")
    {
        std::stringstream ss;
        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossShellState state = PicrossShellState();

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_shell/5_10_hints_only.xml");
        Grid modifiedG = g;
        modifiedG.setCellRange(2, 3, 2, 8, CELL_CHECKED);

        state.mainGrid() = g;
        state.workingGrid() = modifiedG;

        ShellDisplayCommand command = ShellDisplayCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("display azeazeaze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("display nohints azeazeaze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/display_invalid_output.txt");
            REQUIRE(ss.str() == expected);
        }

        SECTION("Display with hints")
        {
            REQUIRE(command.processInput("display", state, s) == SHELL_COMMAND_SUCCESS);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/display_withhints_output.txt");
            REQUIRE(ss.str() == expected);
        }

        SECTION("Display without hints")
        {
            REQUIRE(command.processInput("display nohints", state, s) == SHELL_COMMAND_SUCCESS);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/display_nohints_output.txt");
            REQUIRE(ss.str() == expected);
        }

        REQUIRE(state.mainGrid() == g);
        REQUIRE(state.workingGrid() == modifiedG);
    }
}
