#include "cell_t.hpp"

#include <sstream>
#include <iostream>
#include <stdexcept>

namespace Picross
{
    bool isValidCellValue(cell_t val, bool throwOnFail)
    {
		bool result = (val == CELL_CHECKED) || (val == CELL_CLEARED) || (val == CELL_CROSSED);
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Invalid cell value \"" << val << "\"." << std::endl;
			s << "Possible values are " << CELL_CHECKED << " (checked)," << CELL_CLEARED << " (cleared), and" << CELL_CROSSED << " (crossed)." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
    }

	std::string cellValueToString(cell_t val)
	{
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
				std::stringstream s;
				s << "Unrecognized cell value \"" << val << "\", cannot represent into string." << std::endl;
				throw std::runtime_error(s.str().c_str());
		}
	}
}