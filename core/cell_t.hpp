#ifndef CELL_T
#define CELL_T

#include <cstddef>
#include <string>

namespace Picross
{
    typedef unsigned char cell_t;

    // Cell possible values.
    inline static const cell_t CELL_CROSSED = 0;
    inline static const cell_t CELL_CLEARED = 1;
    inline static const cell_t CELL_CHECKED = 2;

    inline static const std::size_t CELL_T_VALUE_COUNT = 3;
    inline static const cell_t CELL_T_ORDERED_VALUES[CELL_T_VALUE_COUNT] = {
        CELL_CROSSED,
        CELL_CLEARED,
        CELL_CHECKED
    };

    bool isValidCellValue(cell_t val, bool throwOnFail = false);
    std::string cellValueToString(cell_t val);
}

#endif//CELL_T