#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include "../../picross_cli/picross_cli_state.hpp"
#include "../../picross_cli/create_grid_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"

#define TAGS "[cli][cli_command]"

namespace Picross
{
    TEST_CASE("CreateGridCommand end-to-end")
    {
        std::ifstream f = std::ifstream("resources/tests/picross_cli/grid_creation_input.txt", std::ios::in);
        REQUIRE(f);
        std::stringstream ss;

        CLIStreams s = CLIStreams(f, ss, ss);
        PicrossCLIState state = PicrossCLIState();

        CreateGridCommand command = CreateGridCommand();
        command.run(state, s);

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_cli/5_10_hints_only.xml");

        REQUIRE(state.grid() == g);

        f.close();
    }
}
