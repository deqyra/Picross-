#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include <fstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_exit_command.hpp"
#include "../../core/grid.hpp"
#include "../../core/cell_t.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellExitCommand end-to-end")
    {
        std::ifstream f = std::ifstream("resources/tests/picross_shell/exit_input.txt", std::ios::in);
        REQUIRE(f);

        std::stringstream ss;
        CLIStreams s = CLIStreams(f, ss, ss);
        PicrossShellState state = PicrossShellState();

        Grid g = Grid(10, 10);
        Grid modifiedG = g;

        state.mainGrid() = g;
        state.workingGrid() = g;

        ShellExitCommand command = ShellExitCommand();

        REQUIRE(command.processInput("exit", state, s) == SHELL_EXIT);
        REQUIRE(command.processInput("exit aze", state, s) == SHELL_EXIT);

        modifiedG.checkCell(3, 4);
        state.workingGrid() = modifiedG;
        REQUIRE(command.processInput("exit", state, s) == SHELL_COMMAND_SUCCESS);
        REQUIRE(state.mainGrid() == g);
        REQUIRE(state.workingGrid() == modifiedG);

        REQUIRE(command.processInput("exit aze", state, s) == SHELL_EXIT);
        REQUIRE(state.mainGrid() == g);
        REQUIRE(state.workingGrid() == modifiedG);

        std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/exit_output.txt");
        REQUIRE(ss.str() == expected);
    }
}
