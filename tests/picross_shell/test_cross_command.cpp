#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_cross_command.hpp"
#include "../../core/grid.hpp"
#include "../../core/cell_t.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellCrossCommand end-to-end")
    {
        std::stringstream ss;
        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossShellState state = PicrossShellState();

        Grid g = Grid(10, 10);
        Grid modifiedG = g;

        state.mainGrid() = g;
        state.workingGrid() = modifiedG;

        ShellCrossCommand command = ShellCrossCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("cross aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("cross aze rty", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("cross 3 rty", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == modifiedG);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/cross_invalid_output.txt");
            REQUIRE(ss.str() == expected);
        }

        SECTION("Cross single cell")
        {
            REQUIRE(command.processInput("cross 3 5", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.crossCell(3, 5);
        }

        SECTION("Cross row or column range")
        {
            REQUIRE(command.processInput("cross 3 5:8", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(command.processInput("cross 1:4 2", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 3, 5, 8, CELL_CROSSED);
            modifiedG.setCellRange(1, 4, 2, 2, CELL_CROSSED);
        }

        SECTION("Cross row or column range (reversed ranges)")
        {
            REQUIRE(command.processInput("cross 3 8:5", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(command.processInput("cross 4:1 2", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 3, 5, 8, CELL_CROSSED);
            modifiedG.setCellRange(1, 4, 2, 2, CELL_CROSSED);
        }

        SECTION("Cross area range")
        {
            REQUIRE(command.processInput("cross 3:7 5:8", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 7, 5, 8, CELL_CROSSED);
        }

        REQUIRE(state.mainGrid() == g);
        REQUIRE(state.workingGrid() == modifiedG);
    }
}
