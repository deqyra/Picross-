#include "../../lib/catch2/catch2.hpp"

#include <string>
#include "../../string/utility.hpp"

#define TAGS "[string][utility]"

namespace StringUtil
{
    TEST_CASE("popChar pops chars at the end of a string", TAGS)
    {
        std::string str = "azerty\r";
        std::string res = "azerty";
        std::string res2 = "azert";

        REQUIRE(popChar(str, '\r'));
        REQUIRE(str == res);
        REQUIRE(popChar(str, 'y'));
        REQUIRE(str == res2);
    }

    TEST_CASE("popChar does not pop chars in the middle of a string", TAGS)
    {
        std::string str = "azerty\r";
        std::string res = str;

        REQUIRE_FALSE(popChar(str, 'z'));
        REQUIRE(str == res);
        REQUIRE_FALSE(popChar(str, 'y'));
        REQUIRE(str == res);
    }


    TEST_CASE("popChar returns false on an empty string", TAGS)
    {
        std::string str = "";

        REQUIRE_FALSE(popChar(str, 'a'));
        REQUIRE(str == "");
    }

    TEST_CASE("popString pops a substring at the end of a string, including single characters", TAGS)
    {
        std::string str = "azerty\r";
        std::string res = "azert";
        std::string res2 = "aze";
        std::string res3 = "az";

        REQUIRE(popString(str, "y\r"));
        REQUIRE(str == res);
        REQUIRE(popString(str, "rt"));
        REQUIRE(str == res2);
        REQUIRE(popString(str, "e"));
        REQUIRE(str == res3);
    }

    TEST_CASE("popString does not pop a substring in the middle of a string", TAGS)
    {
        std::string str = "azerty\r";
        std::string res = str;

        REQUIRE_FALSE(popString(str, "ze"));
        REQUIRE(str == res);
        REQUIRE_FALSE(popString(str, "az"));
        REQUIRE(str == res);
    }

    TEST_CASE("popString pops the whole string when both provided strings are the same", TAGS)
    {
        std::string str = "";
        std::string str2 = "azerty";
        std::string str2Copy = str2;

        REQUIRE(popString(str, ""));
        REQUIRE(str == "");
        REQUIRE(popString(str2, str2));
        REQUIRE(str2 == "");
    }

    TEST_CASE("popString popString is a no-op when the substring isn't contained in the provided string", TAGS)
    {
        std::string str = "azerty";
        std::string res = str;

        REQUIRE_FALSE(popString(str, "qsd"));
        REQUIRE(str == res);
        REQUIRE_FALSE(popString(str, "azertyuiop"));
        REQUIRE(str == res);
    }

    TEST_CASE("stringContains works in the general case", TAGS)
    {
        std::string str = "azertyyy";

        // Inexact count (at least n occurrences)
        REQUIRE(stringContains(str, 'y', 0, false));
        REQUIRE(stringContains(str, 'y', 1, false));
        REQUIRE(stringContains(str, 'y', 2, false));
        REQUIRE(stringContains(str, 'y', 3, false));
        REQUIRE_FALSE(stringContains(str, 'y', 4, false));

        // Exact count (exactly n occurrences)
        REQUIRE_FALSE(stringContains(str, 'y', 0, true));
        REQUIRE_FALSE(stringContains(str, 'y', 1, true));
        REQUIRE_FALSE(stringContains(str, 'y', 2, true));
        REQUIRE(stringContains(str, 'y', 3, true));
        REQUIRE_FALSE(stringContains(str, 'y', 4, true));
    }

    TEST_CASE("stringContains works on empty strings", TAGS)
    {
        std::string str = "";

        // Searching for 0 occurrences returns true
        REQUIRE(stringContains(str, 'y', 0, false));
        REQUIRE(stringContains(str, 'y', 0, true));

        // Searching for more than 0 occurrences returns false
        REQUIRE_FALSE(stringContains(str, 'y', 1, false));
        REQUIRE_FALSE(stringContains(str, 'y', 1, true));
    }

    TEST_CASE("stringContains behaves properly on 0-occurrence searches", TAGS)
    {
        std::string str = "azerty";

        // Inexact search always returns true, no matter whether the queried character is contained in the string
        REQUIRE(stringContains(str, 'y', 0, false));
        REQUIRE(stringContains(str, 'w', 0, false));

        // Exact search returns true only if the queried character is not present in the string
        REQUIRE_FALSE(stringContains(str, 'y', 0, true));
        REQUIRE(stringContains(str, 'w', 0, true));
    }

    TEST_CASE("stringIsNum works in the general case")
    {
        std::string numeric = "3216548";
        std::string alphanum = "321aze456";
        std::string alphabetic = "azeqsdwxc";

        REQUIRE(stringIsNum(numeric));
        REQUIRE_FALSE(stringIsNum(alphanum));
        REQUIRE_FALSE(stringIsNum(alphabetic));
    }

    TEST_CASE("stringIsNum returns false on empty strings")
    {
        REQUIRE_FALSE(stringIsNum(""));
    }
}