#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_hint_coherence_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellHintCoherenceCommand end-to-end", TAGS)
    {
        std::stringstream ss;
        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossShellState state = PicrossShellState();

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_shell/5_10_hints_only.xml");

        state.mainGrid() = g;
        state.workingGrid() = g;

        ShellHintCoherenceCommand command = ShellHintCoherenceCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("hintCoherence azeazeaze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == g);
        }

        SECTION("Valid arguments")
        {
            REQUIRE(command.processInput("hintCoherence", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == g);

            state.mainGrid().setColHints(4, {1, 3});
            REQUIRE(command.processInput("hintCoherence", state, s) == SHELL_COMMAND_SUCCESS);
        }
    }
}
