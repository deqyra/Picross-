#ifndef CORE__CELL_T
#define CORE__CELL_T

#include <cstddef>
#include <string>

namespace Picross
{
    typedef unsigned char cell_t;

    // Cell possible values
    inline static const cell_t CELL_CROSSED = 0;
    inline static const cell_t CELL_CLEARED = 1;
    inline static const cell_t CELL_CHECKED = 2;

    // Type meta-information
    inline static const std::size_t CELL_T_VALUE_COUNT = 3;
    inline static const cell_t CELL_T_ORDERED_VALUES[CELL_T_VALUE_COUNT] = {
        CELL_CROSSED,
        CELL_CLEARED,
        CELL_CHECKED
    };

    // Checks whether provided value is valid for a cell.
    bool isValidCellValue(cell_t val, bool throwOnFail = false);

    // Gives the string representation of a cell value.
    std::string cellValueToString(cell_t val);
}

#endif//CORE__CELL_T