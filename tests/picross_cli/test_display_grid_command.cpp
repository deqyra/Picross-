#include "../../lib/catch2/catch2.hpp"

#include <fstream>
#include <sstream>
#include "../../picross_cli/picross_cli_state.hpp"
#include "../../picross_cli/cli_display_grid_command.hpp"
#include "../../core/grid.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../tools/string_tools.hpp"

#define TAGS "[cli][cli_command]"

namespace Picross
{
    TEST_CASE("CLIDisplayGridCommand end-to-end", TAGS)
    {
        std::stringstream ss;

        CLIStreams s = CLIStreams(ss, ss, ss);
        PicrossCLIState state = PicrossCLIState();

        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g = xml.loadGridFromFile("resources/tests/picross_cli/20_20_solved.xml");
        state.grid() = g;

        CLIDisplayGridCommand command = CLIDisplayGridCommand();
        command.run(state, s);

        std::string expected = StringTools::readFileIntoString("resources/tests/picross_cli/display_grid_output.txt");
        REQUIRE(ss.str() == expected);
    }
}
