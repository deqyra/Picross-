#include "../../lib/catch2/catch2.hpp"

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
            REQUIRE(IterTools::maxIterableLength(vecs) == 10);
        }
    }
}

SCENARIO("Sum of vector sums works properly")
{
    GIVEN("Some vectors in a vector")
    {
        std::vector<std::vector<int>> vectors;
        vectors.push_back({3, 5, 2});
        vectors.push_back({1, 4, 6});
        vectors.push_back({2, 7});
        vectors.push_back({9, 12, 5, 8});

        THEN("The correct sum is returned")
        {
            REQUIRE(IterTools::sum2NestedIterables<int>(vectors) == 64);
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
            REQUIRE(IterTools::indexOfMaxElement(vec) == 5);
            vec[3] = 100;
            REQUIRE(IterTools::indexOfMaxElement(vec) == 3);
        }
    }

    AND_GIVEN("An int array")
    {
        THEN("Max element is found correctly")
        {
            REQUIRE(IterTools::indexOfMaxElement(arr, 7) == 5);
            arr[3] = 100;
            REQUIRE(IterTools::indexOfMaxElement(arr, 7) == 3);
        }
    }
}
