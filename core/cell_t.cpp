#include "cell_t.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "exceptions/invalid_cell_value_error.hpp"
#include "exceptions/unrecognized_cell_value_error.hpp"

namespace Picross
{
    bool isValidCellValue(const cell_t& val, bool throwOnFail)
    {
		// Look through all registered values.
		auto it = std::find(std::begin(CELL_T_ORDERED_VALUES), std::end(CELL_T_ORDERED_VALUES), val);
		bool found = it != std::end(CELL_T_ORDERED_VALUES);

		// Throw if asked to.
		if (throwOnFail && !found)
		{
			std::string s = "Invalid cell value " + cellNumericValueToString(val) + ".";
			throw InvalidCellValueError(s);
		}

		return found;
    }

	std::string cellValueName(cell_t val)
	{
		// Auto-throw if value is invalid.
		isValidCellValue(val, true);

		// If valid, the value name is reachable in the array at the value index.
		return CELL_T_ORDERED_NAMES[val];
	}

	int cellValueToInt(cell_t val)
	{
		// Clear all bits but keep the 8 LSB (cell_t is an unsigned char = 1 byte).
		return static_cast<int>(val) & 0xFF;
	}

	std::string cellNumericValueToString(cell_t val)
	{
		return std::to_string(cellValueToInt(val));
	}

	std::string cellValueToString(cell_t val)
	{
		// Auto-throw if value is invalid.
		isValidCellValue(val, true);
		return CELL_T_ORDERED_STRINGS[val];
	}
} // namespace Picross

namespace std
{
    string to_string(Picross::cell_t val)
	{
		return Picross::cellValueToString(val);
	}
} // namespace std
