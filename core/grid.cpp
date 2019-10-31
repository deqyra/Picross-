#include "grid.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

#include "cell_t.hpp"
#include "utility.hpp"

namespace Picross
{
	Grid::Grid(int width, int height) :
		_width(width),
		_height(height),
		_content(width * height, CELL_CLEARED),
		_horizontalHints(height, std::vector<int>()),
		_verticalHints(width, std::vector<int>())
	{

	}

	Grid::Grid(int width, int height, std::vector<std::vector<int>> horizontalHints, std::vector<std::vector<int>> verticalHints) :
		_width(width),
		_height(height),
		_content(width * height, CELL_CLEARED),
		_horizontalHints(horizontalHints),
		_verticalHints(verticalHints)
	{
		// Throw and cancel object creation if any hints are invalid.
		// Catch all exceptions and throw a big one with the text of all of them.
		bool exceptionCaught = false;
		std::stringstream exceptionText;

		for (auto it = _horizontalHints.begin(); it != _horizontalHints.end(); it++)
		{
			try
			{
				areValidRowHints((*it), true);
			}
			catch(const std::runtime_error& e)
			{
				exceptionCaught = true;
				exceptionText << e.what();
			}
		}

		for (auto it = _verticalHints.begin(); it != _verticalHints.end(); it++)
		{
			try
			{
				areValidColHints((*it), true);
			}
			catch(const std::runtime_error& e)
			{
				exceptionCaught = true;
				exceptionText << e.what();
			}
		}

		if (exceptionCaught)
		{
			throw std::runtime_error(exceptionText.str().c_str());
		}
	}

	int Grid::getWidth() const
	{
		return _width;
	}

	int Grid::getHeight() const
	{
		return _height;
	}

	std::vector<cell_t> Grid::getRow(int row) const
	{
		// This will throw if the check fails (last parameter).
		isValidRow(row, true);

		auto begin = _content.begin() + (_width * row);
		return std::vector<cell_t>(begin, begin + _width);
	}

	std::vector<cell_t> Grid::getCol(int col) const
	{
		// This will throw if the check fails (last parameter).
		isValidCol(col, true);

		std::vector<cell_t> columnVector(_height, CELL_CLEARED);

		auto it = _content.begin() + col;
		for (int i = 0; i < _height; i++)
		{
			columnVector[i] = (*it);
			it += _width;
		}
		return columnVector;
	}

	std::vector<std::vector<int>> Grid::getAllRowHints() const
	{
		return _horizontalHints;
	}

	std::vector<int> Grid::getRowHints(int row) const
	{
		// This will throw if the check fails (last parameter).
		isValidRow(row, true);

		return _horizontalHints[row];
	}

	std::vector<std::vector<int>> Grid::getAllColHints() const
	{
		return _verticalHints;
	}

	std::vector<int> Grid::getColHints(int col) const
	{
		// This will throw if the check fails (last parameter).
		isValidCol(col, true);

		return _verticalHints[col];
	}

	cell_t Grid::getCell(int row, int col) const
	{
		// This will throw if the check fails (last parameter).
		isValidCell(row, col, true);

		return _content[(row * _width) + col];
	}

	void Grid::setCell(int row, int col, cell_t val)
	{
		// This will throw if the checks fail (last parameter).
		isValidCell(row, col, true);
		isValidCellValue(val, true);

		_content[(row * _width) + col] = val;
	}

	void Grid::checkCell(int row, int col)
	{
		// This will throw if the checks fail (last parameter).
		isValidCell(row, col, true);

		_content[(row * _width) + col] = CELL_CHECKED;
	}

	void Grid::crossCell(int row, int col)
	{
		// This will throw if the checks fail (last parameter).
		isValidCell(row, col, true);

		_content[(row * _width) + col] = CELL_CROSSED;
	}

	void Grid::clearCell(int row, int col)
	{
		// This will throw if the checks fail (last parameter).
		isValidCell(row, col, true);

		_content[(row * _width) + col] = CELL_CLEARED;
	}

	void Grid::setRowHints(int row, std::vector<int> hints)
	{
		// These checks will throw on fails (last parameter).
		isValidRow(row, true);
		areValidRowHints(hints, true);

		_horizontalHints[row] = hints;
	}

	void Grid::setColHints(int col, std::vector<int> hints)
	{
		// These checks will throw on fails (last parameter).
		isValidCol(col, true);
		areValidColHints(hints, true);

		_verticalHints[col] = hints;
	}

	void Grid::setAllRowHints(std::vector<std::vector<int>> hints)
	{
		if (hints.size() != _height)
		{
			std::stringstream s;
			s << "Horizontal hints vector size (" << hints.size() << ") must be the same as grid height (" << _height << ")." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}

		for (int i = 0; i < hints.size(); i++)
		{
			setRowHints(i, hints[i]);
		}
	}

	void Grid::setAllColHints(std::vector<std::vector<int>> hints)
	{
		if (hints.size() != _width)
		{
			std::stringstream s;
			s << "Vertical hints vector size (" << hints.size() << ") must be the same as grid width (" << _width << ")." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}

		for (int i = 0; i < hints.size(); i++)
		{
			setColHints(i, hints[i]);
		}
	}

	bool Grid::isValidRow(int row, bool throwOnFail) const
	{
		bool result = row >= 0 && row < _height;
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Invalid row " << row << " for grid with height " << _height << "." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
	}

	bool Grid::isValidCol(int col, bool throwOnFail) const
	{
		bool result = col >= 0 && col < _width;
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Invalid column " << col << " for grid with width " << _width << "." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
	}

	bool Grid::isValidCell(int row, int col, bool throwOnFail) const
	{
		bool result = isValidRow(row) && isValidCol(col);
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Invalid cell (" << row << ", " << col << ") for grid with dimensions (" << _height << ", " << _width << ")." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
	}

    cell_t Grid::mostPresentState() const
    {
        // Array to count occurrences of each cell value.
        int counts[CELL_T_VALUE_COUNT] = {0};

        // Traverse provided grid and increase counts accordingly.
        for (int row = 0; row < getHeight(); row++)
        {
            for (int col = 0; col < getWidth(); col++)
            {
                counts[getCell(row, col)]++;

                // This works because the underlying values of cell values
                // start from 0 and go up in 1-increments, which corresponds
                // to the indexing of the `counts` array.
                //
                // As a side effect, the layout of that array exactly matches
                // that of CELL_T_ORDERED_VALUES, which allows to map counts
                // and values on the same index (used in the return statement below).
            }
        }

        // Find max index and return appropriate value.
        int maxIndex = indexOfMaxElement(counts, CELL_T_VALUE_COUNT);
        return CELL_T_ORDERED_VALUES[maxIndex];
    }

	bool Grid::areValidRowHints(const std::vector<int>& hints, bool throwOnFail) const
	{
		int space = minimumSpaceFromHints(hints);
		bool result = space <= _width;
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Hints " << vectorToString(hints, ", ", "(", ")") << " require minimum space " << space << " which exceeds grid width (" << _width << "), and thus are invalid." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
	}

	bool Grid::areValidColHints(const std::vector<int>& hints, bool throwOnFail) const
	{
		int space = minimumSpaceFromHints(hints);
		bool result = space <= _height;
		if (throwOnFail && !result)
		{
			std::stringstream s;
			s << "Hints " << vectorToString(hints, ", ", "(", ")") << " require minimum space " << space << " which exceeds grid height (" << _height << "), and thus are invalid." << std::endl;
			throw std::runtime_error(s.str().c_str());
		}
		return result;
	}

	bool operator==(const Grid& lhs, const Grid& rhs)
	{
		if (lhs._width != rhs._width) return false;
		if (lhs._height != rhs._height) return false;
		if (lhs._horizontalHints != rhs._horizontalHints) return false;
		if (lhs._verticalHints != rhs._verticalHints) return false;
		if (lhs._content != rhs._content) return false;

		return true;
	}
}