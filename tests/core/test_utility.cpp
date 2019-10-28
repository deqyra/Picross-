#include "../../lib/catch2/catch2.hpp"

#include <string>
#include <vector>
#include <stdexcept>

#include "../template_scenario.hpp"
#include "../../core/cell_t.hpp"
#include "../../core/grid.hpp"
#include "../../core/utility.hpp"

#define TAGS "[core][utility]"

namespace Picross
{
    SCENARIO("Strings are properly parsed into int vectors", TAGS)
    {
        GIVEN("A well-formed space-separated string")
        {
            std::string str = "5 2 1";

            THEN("String is correctly parsed into a vector")
            {
                std::vector<int> expected = {5, 2, 1};
                REQUIRE(stringToIntVector(str) == expected);
            }
        }

        GIVEN("A string with way too many spaces")
        {
            std::string str = "     5   2        1      ";

            THEN("String is correctly parsed into a vector")
            {
                std::vector<int> expected = {5, 2, 1};
                REQUIRE(stringToIntVector(str) == expected);
            }
        }

        GIVEN("An ill-formed string (incorrect delimiter)")
        {
            std::string str = "5;2;1";

            THEN("An exception is thrown")
            {
                REQUIRE_THROWS_AS(stringToIntVector(str), std::runtime_error);
            }
        }
    }

    SCENARIO("Minimum grid space required can be computed from hint sequence", TAGS)
    {
        GIVEN("A hint sequence")
        {
            std::vector<int> seq = {5, 2, 1};

            THEN("The correct minimum space is returned")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 10);
            }
        }

        AND_GIVEN("Another hint sequence")
        {
            std::vector<int> seq = {10, 32, 63};

            THEN("The correct minimum space is returned")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 107);
            }
        }

        AND_GIVEN("A hint sequence with negative hints")
        {
            std::vector<int> seq = {4, -6, 10};

            THEN("A minimum space is returned even though it doesn't make sense semantically")
            {
                REQUIRE(minimumSpaceFromHints(seq) == 10);
            }
        }
    }

    SCENARIO("Strings are properly tokenized", TAGS)
    {
        GIVEN("Any string")
        {
            std::string str = "Hello, world. Bleep bloop, am robot.";

            THEN("String is properly tokenized on a delimiter")
            {
                std::vector<std::string> spaceTokens = {"Hello,", "world.", "Bleep", "bloop,", "am", "robot."};
                REQUIRE(tokenizeString(str, ' ', false) == spaceTokens);
            }

            AND_THEN("There are no empty tokens (discarding them doesn't affect the result)")
            {
                std::vector<std::string> spaceTokens = {"Hello,", "world.", "Bleep", "bloop,", "am", "robot."};
                REQUIRE(tokenizeString(str, ' ', true) == spaceTokens);
            }

            AND_THEN("String is properly tokenized on commas")
            {
                std::vector<std::string> commaTokens = {"Hello", " world. Bleep bloop", " am robot."};
                REQUIRE(tokenizeString(str, ',', false) == commaTokens);
            }

            AND_THEN("String is properly tokenized on 'e's")
            {
                std::vector<std::string> eTokens = {"H", "llo, world. Bl", "", "p bloop, am robot."};
                REQUIRE(tokenizeString(str, 'e', false) == eTokens);
            }

            AND_THEN("Empty tokens are properly discarded when asked to")
            {
                std::vector<std::string> eTokens = {"H", "llo, world. Bl", "p bloop, am robot."};
                REQUIRE(tokenizeString(str, 'e', true) == eTokens);
            }

            AND_THEN("String ending with delimiter does not result in last token being empty")
            {
                std::vector<std::string> dotTokens = {"Hello, world", " Bleep bloop, am robot"};
                REQUIRE(tokenizeString(str, '.', false) == dotTokens);
            }
        }
    }

    SCENARIO("Multiline string concatenation works properly", TAGS)
    {
        GIVEN("A couple multiline strings of equal numbers of lines")
        {
            std::string str1 =  "String 1 part 1 \n"
                                "String 1 part 2 \n"
                                "String 1 part 3 \n";

            std::string str2 =  "String 2 part 1\n"
                                "String 2 part 2\n"
                                "String 2 part 3\n";

            std::string res =   "String 1 part 1 String 2 part 1\n"
                                "String 1 part 2 String 2 part 2\n"
                                "String 1 part 3 String 2 part 3\n";

            REQUIRE(multilineConcatenation(str1, str2) == res);
        }
    }

    TEMPLATE_SCENARIO("Vectors can be formatted into a custom string", TAGS, int, char, std::string)
    {
        GIVEN("Some vector")
        {
            TestType val = TestType();
            std::vector<TestType> vec = {val, val};

            THEN("Vector is properly formatted with no surrounding characters")
            {
                std::stringstream s;
                s << val << " " << val;

                REQUIRE(vectorToString(vec) == s.str());
            }

            AND_THEN("Vector is properly formatted with single surrounding characters")
            {
                std::stringstream s;
                s << "<[" << val << "]-[" << val << "]>";

                REQUIRE(vectorToString(vec, "-", "<", ">", "[", "]") == s.str());
            }

            AND_THEN("Vector is properly formatted with surrounding strings")
            {
                std::stringstream s;
                s << "<~(['" << val << "'] # ['" << val << "'])~>";

                REQUIRE(vectorToString(vec, " # ", "<~(", ")~>", "['", "']") == s.str());
            }
        }
    }

    SCENARIO("Maximum vector length can be found among vectors in a vector", TAGS)
    {
        GIVEN("A vector of vectors")
        {
            std::vector<std::vector<int>> vecs = {
                std::vector<int>(3),
                std::vector<int>(5),
                std::vector<int>(4),
                std::vector<int>(10),
                std::vector<int>(7)
            };

            THEN("Maximum length is correctly found")
            {
                REQUIRE(maxVectorLength(vecs) == 10);
            }
        }
    }

    SCENARIO("Index of max element can be found within a vectors and arrays alike")
    {
        int arr[7] = {2, 9, 8, 11, 4, 17, 7};
        std::vector<int> vec(arr, arr + sizeof(arr) / sizeof(int));

        GIVEN("An int vector")
        {
            THEN("Max element is found correctly")
            {
                REQUIRE(indexOfMaxElement(vec) == 5);
                vec[3] = 100;
                REQUIRE(indexOfMaxElement(vec) == 3);
            }
        }

        AND_GIVEN("An int array")
        {
            THEN("Max element is found correctly")
            {
                REQUIRE(indexOfMaxElement(arr, 7) == 5);
                arr[3] = 100;
                REQUIRE(indexOfMaxElement(arr, 7) == 3);
            }
        }
    }
}