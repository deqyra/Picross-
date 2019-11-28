#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_commit_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellCommitCommand end-to-end", TAGS)
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

        ShellCommitCommand command = ShellCommitCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("commit azeazeaze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == modifiedG);
        }

        SECTION("Valid arguments")
        {
            REQUIRE(command.processInput("commit", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(state.mainGrid() == modifiedG);
            REQUIRE(state.workingGrid() == modifiedG);
        }
    }
}
