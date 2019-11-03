#include "cell_t.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>

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
			std::stringstream s;
			s << "Invalid cell value \"" << val << "\"." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}

		return found;
    }

	std::string cellValueToString(cell_t val)
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
				std::stringstream s;
				s << "Unrecognized cell value \"" << val << "\", cannot represent into string." << std::endl;
				throw std::runtime_error(s.str().c_str());
		}
	}
}