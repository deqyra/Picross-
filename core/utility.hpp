#ifndef CORE_UTILITY_HPP
#define CORE_UTILITY_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "cell_t.hpp"
#include "grid.hpp"

namespace Picross
{
    int minimumSpaceFromHints(const std::vector<int>& hints);
    std::vector<int> stringToHintVector(const std::string& str, char delimiter = ' ');
    std::vector<std::string> tokenizeString(const std::string& str, char delimiter);
    std::string multilineConcatenation(std::string first, std::string second);
    cell_t mostPresentState(const Grid& grid);

    template<typename T>
    std::string vectorToString(const std::vector<T>& elements, std::string delimiter = " ", std::string opening = "", std::string closing = "")
    {
        std::stringstream s;
        s << opening;
        for (auto it = elements.begin(); it != elements.end() - 1; it++)
        {
            s << (*it) << delimiter;
        }
        s << elements.back() << closing;

        return s.str();
    }

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

    template<typename T>
    int indexOfMaxElement(std::vector<T> array)
    {
        auto first = array.begin();
        auto last = array.end();
        return std::distance(first, std::max_element(first, last));
    }

    template<typename T>
    int indexOfMaxElement(T* array, int length)
    {
        auto first = array;
        auto last = array + (sizeof(T) * length);
        return std::distance(first, std::max_element(first, last)) / sizeof(T);
    }
}

#endif//CORE_UTILITY_HPP