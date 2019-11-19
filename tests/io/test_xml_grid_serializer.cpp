#include "../../lib/catch2/catch2.hpp"

#include "generate_static_grids.hpp"
#include "../../io/xml_grid_serializer.hpp"
#include "../../core/grid.hpp"

#define TAGS "[io][xml][grid][serialization]"

namespace Picross
{
    TEST_CASE("XML deserialization", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        Grid g(0, 0);
        REQUIRE_NOTHROW(g = xml.loadGridFromFile("resources/tests/io/10_10_partial.xml"));
        REQUIRE(g == generate10x10PartialGrid());
    }

    TEST_CASE("XML serialization", TAGS)
    {
        XMLGridSerialzer xml = XMLGridSerialzer();
        REQUIRE_NOTHROW(xml.saveGridToFile(generate10x10PartialGrid(), "resources/tests/io/output.xml"));

        // If the grid was properly saved, the loaded grid should be the same.
        // (assuming deserialization works, which was made sure of by the above test case)
        Grid g = xml.loadGridFromFile("resources/tests/io/output.xml");
        REQUIRE(g == generate10x10PartialGrid());
    }
}
