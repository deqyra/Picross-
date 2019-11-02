#include "../../lib/catch2/catch2.hpp"

#include <string>
#include "../../string/utility.hpp"

#define TAGS "[string][utility]"

namespace StringUtil
{
    SCENARIO("popChar works")
    {
        GIVEN("Some strings")
        {
            std::string str1Res = "azerty";
            std::string str1 = "azerty\r";
            std::string str2Res = "azerty\ruiop";
            std::string str2 = "azerty\ruiop";
            std::string str3Res = "azertyuiop";
            std::string str3 = "azertyuiop";
            std::string str4Res = "jean-jacque";
            std::string str4 = "jean-jacques";
            std::string str5Res = "";
            std::string str5 = "";

            THEN("The given char is popped if located at the end of the string")
            {
                REQUIRE(popChar(str1, '\r'));
                REQUIRE(str1 == str1Res);

                REQUIRE_FALSE(popChar(str2, '\r'));
                REQUIRE(str2 == str2Res);

                REQUIRE_FALSE(popChar(str3, '\r'));
                REQUIRE(str3 == str3Res);

                REQUIRE(popChar(str4, 's'));
                REQUIRE(str4 == str4Res);

                REQUIRE_FALSE(popChar(str5, 's'));
                REQUIRE(str5 == str5Res);
            }
        }
    }

    SCENARIO("popString works")
    {
        GIVEN("Some strings")
        {
            std::string str1Res = "azert";
            std::string str1 = "azerty\r";
            std::string str2Res = "azerty\ruiop";
            std::string str2 = "azerty\ruiop";
            std::string str3Res = "azertyuiop";
            std::string str3 = "azertyuiop";
            std::string str4Res = "jean-jac";
            std::string str4 = "jean-jacques";

            THEN("The substring is popped if located at the end of the string")
            {
                REQUIRE(popString(str1, "y\r"));
                REQUIRE(str1 == str1Res);

                REQUIRE_FALSE(popString(str2, "y\r"));
                REQUIRE(str2 == str2Res);

                REQUIRE_FALSE(popString(str3, "y\r"));
                REQUIRE(str3 == str3Res);

                REQUIRE(popString(str4, "ques"));
                REQUIRE(str4 == str4Res);

                REQUIRE_FALSE(popString(str3, "azertyuiopaze"));
                REQUIRE(str3 == str3Res);

                REQUIRE(popString(str3, str3));
                REQUIRE(str3 == "");
            }
        }
    }

    SCENARIO("stringContains works")
    {
        GIVEN("Some strings")
        {
            std::string str0 = "azertyyy";

            THEN("The correct value is returned")
            {
                REQUIRE(stringContains(str0, 'y', 0, false));
                REQUIRE(stringContains(str0, 'y', 1, false));
                REQUIRE(stringContains(str0, 'y', 2, false));
                REQUIRE(stringContains(str0, 'y', 3, false));
                REQUIRE_FALSE(stringContains(str0, 'y', 4, false));

                REQUIRE_FALSE(stringContains(str0, 'y', 0, true));
                REQUIRE_FALSE(stringContains(str0, 'y', 1, true));
                REQUIRE_FALSE(stringContains(str0, 'y', 2, true));
                REQUIRE(stringContains(str0, 'y', 3, true));
                REQUIRE_FALSE(stringContains(str0, 'y', 4, true));
            }
        }
    }

    SCENARIO("stringIsNum works")
    {
        GIVEN("Some strings")
        {
            std::string numeric = "3216548";
            std::string alphanum = "321aze456";
            std::string alphabetic = "azeqsdwxc";

            THEN("Strings are properly detected as being numeric or not")
            {
                REQUIRE(stringIsNum(numeric));
                REQUIRE_FALSE(stringIsNum(alphanum));
                REQUIRE_FALSE(stringIsNum(alphabetic));
            }
        }
    }
}