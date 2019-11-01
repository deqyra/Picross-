#include "../../lib/catch2/catch2.hpp"

#include <string>
#include "../../string/utility.hpp"

#define TAGS "[string][utility]"

namespace StringUtils
{
    SCENARIO("popCR works")
    {
        GIVEN("Some strings")
        {
            std::string str1 = "azerty\r";
        }
    }
}