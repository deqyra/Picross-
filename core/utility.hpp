#ifndef CORE__UTILITY_HPP
#define CORE__UTILITY_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "cell_t.hpp"
#include "grid.hpp"
#include "../string/utility.hpp"

namespace Picross
{
    // Returns the minimum cell length needed for a hint sequence to be satisfied.
    int minimumSpaceFromHints(const std::vector<int>& hints);

    // Parses an int sequence from a string and returns it in a vector.
    std::vector<int> stringToIntVector(const std::string& str, char delimiter = ' ');

    // Takes in two multi-line strings, concatenates them line by line and returns the result in one string.
    std::string multilineConcatenation(std::string first, std::string second);

    // Returns a string representation of the contents of a vector.
    template<typename T>
    std::string vectorToString(
        const std::vector<T>& elements, std::string delimiter = " ",
        std::string opening = "",       std::string closing = "",
        std::string elementPrefix = "", std::string elementSuffix = "")
    {
        std::string s = opening;

        if (elements.size())
        {
            for (auto it = elements.begin(); it != elements.end() - 1; it++)
            {
                s += elementPrefix + std::to_string(*it) + elementSuffix + delimiter;
            }
            s += elementPrefix + std::to_string(elements.back()) + elementSuffix + closing;
        }
        else
        {
            s += closing;
        }

        // Output example:
        // vectorToString({5, 2, 1}, "-", "<", ">", "[", "]") == "<[5]-[2]-[1]>"

        return s;
    }

    // Given a vector of vectors, returns the maximum length of the contained vectors.
    template<typename T>
    int maxVectorLength(std::vector<std::vector<T>> vectors)
    {
        int maxSize = 0;
        for (auto it = vectors.begin(); it != vectors.end(); it++)
        {
            int size = it->size();
            if (size > maxSize)
            {
                maxSize = size;
            }
        }

        return maxSize;
    }

    // Given a vector of comparable values, returns the index of the element with the maximum value.
    template<typename T>
    int indexOfMaxElement(std::vector<T> array)
    {
        // Type-aware distance between vector start pointer and max element pointer = max index
        return std::distance(array.begin(), std::max_element(array.begin(), array.end()));
    }

    // Given an array of comparable values and its length, returns the index of the element with the maximum value.
    template<typename T>
    int indexOfMaxElement(T* array, int length)
    {
        // Distance between array start pointer and max element pointer = max index * type size
        return std::distance(array, std::max_element(array, array + length));
    }
}

#endif//CORE__UTILITY_HPP