#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_clear_command.hpp"
#include "../../core/grid.hpp"
#include "../../core/cell_t.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellClearCommand end-to-end", TAGS)
    {
        std::stringstream ss;
        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossShellState state = PicrossShellState();

        Grid g = Grid(10, 10);
        Grid modifiedG = g;
        modifiedG.setCellRange(0, 9, 0, 9, CELL_CHECKED);

        state.mainGrid() = g;
        state.workingGrid() = modifiedG;

        ShellClearCommand command = ShellClearCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("clear aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("clear aze rty", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("clear 3 rty", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == modifiedG);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/clear_invalid_output.txt");
            REQUIRE(ss.str() == expected);
        }

        SECTION("Clear single cell")
        {
            REQUIRE(command.processInput("clear 3 5", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.clearCell(3, 5);
        }

        SECTION("Clear row or column range")
        {
            REQUIRE(command.processInput("clear 3 5:8", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(command.processInput("clear 1:4 2", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 3, 5, 8, CELL_CLEARED);
            modifiedG.setCellRange(1, 4, 2, 2, CELL_CLEARED);
        }

        SECTION("Clear row or column range (reversed ranges)")
        {
            REQUIRE(command.processInput("clear 3 8:5", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(command.processInput("clear 4:1 2", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 3, 5, 8, CELL_CLEARED);
            modifiedG.setCellRange(1, 4, 2, 2, CELL_CLEARED);
        }

        SECTION("Clear area range")
        {
            REQUIRE(command.processInput("clear 3:7 5:8", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setCellRange(3, 7, 5, 8, CELL_CLEARED);
        }

        REQUIRE(state.mainGrid() == g);
        REQUIRE(state.workingGrid() == modifiedG);
    }
}
