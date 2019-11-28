#include "../../lib/catch2/catch2.hpp"

#include <sstream>
#include "../../tools/cli/cli_streams.hpp"
#include "../../tools/micro_shell/micro_shell_codes.hpp"
#include "../../picross_shell/picross_shell_state.hpp"
#include "../../picross_shell/shell_hints_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[shell][shell_command]"

namespace Picross
{
    TEST_CASE("ShellHintsCommand end-to-end")
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

        ShellHintsCommand command = ShellHintsCommand();

        SECTION("Bad arguments")
        {
            REQUIRE(command.processInput("hints aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("hints v aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("hints v aze aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("hints v 3 aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("hints generate aze", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(command.processInput("hints h 3 5;5", state, s) == SHELL_COMMAND_BAD_ARGUMENTS);
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == modifiedG);
            std::string expected = StringTools::readFileIntoString("resources/tests/picross_shell/hints_invalid_output.txt");
            REQUIRE(ss.str() == expected);
        }

        SECTION("Specify new hints")
        {
            REQUIRE(command.processInput("hints h 3 2;4;2", state, s) == SHELL_COMMAND_SUCCESS);
            REQUIRE(command.processInput("hints v 2 2;2", state, s) == SHELL_COMMAND_SUCCESS);

            modifiedG.setRowHints(3, {2, 4, 2});
            modifiedG.setColHints(2, {2, 2});
            
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == modifiedG);
        }

        SECTION("Generate hints from state")
        {
            Grid completedNoHints = xml.loadGridFromFile("resources/tests/picross_shell/5_10_completed_nohints.xml");
            Grid completed = xml.loadGridFromFile("resources/tests/picross_shell/5_10_completed.xml");
            state.workingGrid() = completedNoHints;

            REQUIRE(command.processInput("hints generate", state, s) == SHELL_COMMAND_SUCCESS);
            
            REQUIRE(state.mainGrid() == g);
            REQUIRE(state.workingGrid() == completed);
        }
    }
}
