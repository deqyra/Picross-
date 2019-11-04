#include "cell_t.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>
#include "exceptions/invalid_cell_value_error.hpp"

namespace Picross
{
    bool isValidCellValue(cell_t val, bool throwOnFail)
    {
		// Look through all registered values
		bool found = false;
		for (int i = 0; i < CELL_T_VALUE_COUNT; i++)
		{
			if (val == CELL_T_ORDERED_VALUES[i])
			{
				found = true;
				break;
			}
		}

		// Throw if asked to
		if (throwOnFail && !found)
		{
			std::string s = "Invalid cell value " + cellValueToString(val) + ".\n";
			throw InvalidCellValueError(s);
		}

		return found;
    }

	std::string cellValueName(cell_t val)
	{
		// Throw if value is invalid
		isValidCellValue(val, true);

		switch (val)
		{
			case CELL_CHECKED:
				return "CELL_CHECKED";
				break;
			case CELL_CLEARED:
				return "CELL_CLEARED";
				break;
			case CELL_CROSSED:
				return "CELL_CROSSED";
				break;
			default:
				// A value may be valid as far as the type is concerned, but not handled in here. Report this properly.
				std::string s = "Unrecognized cell value " + cellValueToString(val) + ", name unknown.\n";
				throw InvalidCellValueError(s);
		}
	}

	int cellValueToInt(cell_t val)
	{
		return static_cast<int>(val) & 0xFF;
	}

	std::string cellValueToString(cell_t val)
	{
		return std::to_string(cellValueToInt(val));
	}
}