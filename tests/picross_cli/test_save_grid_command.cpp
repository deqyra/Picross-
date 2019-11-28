#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include "../../picross_cli/picross_cli_state.hpp"
#include "../../picross_cli/cli_save_grid_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[cli][cli_command]"

namespace Picross
{
    TEST_CASE("CLISaveGridCommand end-to-end", TAGS)
    {
        std::ifstream f = std::ifstream("resources/tests/picross_cli/save_grid_input.txt", std::ios::in);
        REQUIRE(f);
        std::stringstream ss;

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_cli/5_10_completed.xml");

        CLIStreams s = CLIStreams(f, ss, ss);
        PicrossCLIState state = PicrossCLIState();
        state.grid() = g;

        CLISaveGridCommand command = CLISaveGridCommand();
        command.run(state, s);

        Grid gSaved = xml.loadGridFromFile("resources/tests/picross_cli/saved_grid.xml");

        REQUIRE(gSaved == g);

        f.close();
    }
}
