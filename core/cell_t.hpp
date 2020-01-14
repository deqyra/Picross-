#ifndef CORE__CELL_T
#define CORE__CELL_T

#include <cstddef>
#include <string>
#include <array>

namespace Picross
{
    typedef unsigned char cell_t;

    // Cell possible values
    inline static const cell_t CELL_CROSSED = 0;
    inline static const cell_t CELL_CLEARED = 1;
    inline static const cell_t CELL_CHECKED = 2;

    // Cell value names
    inline static const std::string CELL_CROSSED_NAME = "CELL_CROSSED";
    inline static const std::string CELL_CLEARED_NAME = "CELL_CLEARED";
    inline static const std::string CELL_CHECKED_NAME = "CELL_CHECKED";

    // Associated display strings
    inline static const std::string CELL_CROSSED_STRING = "×";
    inline static const std::string CELL_CLEARED_STRING = " ";
    inline static const std::string CELL_CHECKED_STRING = "■";

    // Type meta-information
    inline static const std::size_t CELL_T_VALUE_COUNT = 3;
    inline static const std::array<cell_t, CELL_T_VALUE_COUNT> CELL_T_ORDERED_VALUES = {
        CELL_CROSSED,
        CELL_CLEARED,
        CELL_CHECKED
    };
    inline static const std::array<std::string, CELL_T_VALUE_COUNT> CELL_T_ORDERED_STRINGS = {
        CELL_CROSSED_STRING,
        CELL_CLEARED_STRING,
        CELL_CHECKED_STRING
    };
    inline static const std::array<std::string, CELL_T_VALUE_COUNT> CELL_T_ORDERED_NAMES = {
        CELL_CROSSED_NAME,
        CELL_CLEARED_NAME,
        CELL_CHECKED_NAME
    };

    // Checks whether provided value is valid for a cell.
    bool isValidCellValue(const cell_t& val, bool throwOnFail = false);

    // Gives the name of a cell value.
    std::string cellValueName(cell_t val);

    // Gives the int representation of a cell value.
    int cellValueToInt(cell_t val);

    // Gives the string representation of a numeric cell value.
    std::string cellNumericValueToString(cell_t val);

    // Gives the string representation of a numeric cell value.
    std::string cellValueToString(cell_t val);
}

namespace std
{
    string to_string(Picross::cell_t val);
}

#endif//CORE__CELL_T