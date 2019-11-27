#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include "../../picross_cli/picross_cli_state.hpp"
#include "../../picross_cli/cli_modify_grid_command.hpp"
#include "../../core/grid.hpp"
#include "../../core/cell_t.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../tools/string_tools.hpp"

#define TAGS "[cli][cli_command]"

namespace Picross
{
    TEST_CASE("CLIModifyGridCommand end-to-end")
    {
        std::ifstream f = std::ifstream("resources/tests/picross_cli/modify_grid_input.txt", std::ios::in);
        REQUIRE(f);
        std::stringstream ss;

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_cli/5_10_completed.xml");

        CLIStreams s = CLIStreams(f, ss, ss);
        PicrossCLIState state = PicrossCLIState();
        state.grid() = g;

        CLIModifyGridCommand command = CLIModifyGridCommand();
        command.run(state, s);

        // Check the grid wasn't modified in the state (changes were made but not committed)
        REQUIRE(state.grid() == g);

        command.run(state, s);

        // Check the grid _has_ been modified in the state (committed changes)
        g.setCellRange(0, 4, 0, 9, CELL_CLEARED);
        REQUIRE(state.grid() == g);

        std::string str = StringTools::readFileIntoString("resources/tests/picross_cli/modify_grid_output.txt");
        REQUIRE(str == ss.str());

        f.close();
    }
}
