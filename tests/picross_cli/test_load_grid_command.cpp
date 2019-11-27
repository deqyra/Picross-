#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include "../../picross_cli/picross_cli_state.hpp"
#include "../../picross_cli/cli_load_grid_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[cli][cli_command]"

namespace Picross
{
    TEST_CASE("CLILoadGridCommand end-to-end")
    {
        std::ifstream f = std::ifstream("resources/tests/picross_cli/load_grid_input.txt", std::ios::in);
        REQUIRE(f);
        std::stringstream ss;

        CLIStreams s = CLIStreams(f, ss, ss);
        PicrossCLIState state = PicrossCLIState();

        CLILoadGridCommand command = CLILoadGridCommand();
        command.run(state, s);

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_cli/5_10_completed.xml");

        REQUIRE(state.grid() == g);

        f.close();
    }
}
