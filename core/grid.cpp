#include "grid.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

#include "cell_t.hpp"
#include "utility.hpp"
#include "../tools/exceptions/index_out_of_bounds_error.hpp"
#include "exceptions/invalid_grid_hints_error.hpp"

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
		std::string exceptionText;

		for (auto it = _horizontalHints.begin(); it != _horizontalHints.end(); it++)
		{
			try
			{
				areValidRowHints((*it), true);
			}
			catch(const InvalidGridHintsError& e)
			{
				exceptionCaught = true;
				exceptionText += e.what();
			}
		}

		for (auto it = _verticalHints.begin(); it != _verticalHints.end(); it++)
		{
			try
			{
				areValidColHints((*it), true);
			}
			catch(const InvalidGridHintsError& e)
			{
				exceptionCaught = true;
				exceptionText += e.what();
			}
		}

		if (exceptionCaught)
		{
			throw InvalidGridHintsError(exceptionText);
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
			std::string s = "Horizontal hints vector size (" + std::to_string(hints.size()) + ") must be the same as grid height (" + std::to_string(_height) + ").\n";
			throw InvalidGridHintsError(s);
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
			std::string s = "Vertical hints vector size (" + std::to_string(hints.size()) = ") must be the same as grid width (" + std::to_string(_width) + ").\n";
			throw InvalidGridHintsError(s);
		}

		for (int i = 0; i < hints.size(); i++)
		{
			setColHints(i, hints[i]);
		}
	}

	void Grid::setHintsFromState()
	{
		std::vector<std::vector<int>> newHorizontalHints;
		std::vector<std::vector<int>> newVerticalHints;

		// Fill those two new vectors with hints generated from current columns and rows.
		for (int i = 0; i < _height; i++)
		{
			newHorizontalHints.push_back(hintsFromCells(getRow(i)));
		}

		for (int i = 0; i < _width; i++)
		{
			newVerticalHints.push_back(hintsFromCells(getCol(i)));
		}

		_horizontalHints = newHorizontalHints;
		_verticalHints = newVerticalHints;
	}

	bool Grid::isValidRow(int row, bool throwOnFail) const
	{
		bool result = row >= 0 && row < _height;
		if (throwOnFail && !result)
		{
			std::string s = "Invalid row " + std::to_string(row) + " for grid with height " + std::to_string(_height) + ".\n";
			throw IndexOutOfBoundsError(s);
		}
		return result;
	}

	bool Grid::isValidCol(int col, bool throwOnFail) const
	{
		bool result = col >= 0 && col < _width;
		if (throwOnFail && !result)
		{
			std::string s = "Invalid column " + std::to_string(col) + " for grid with width " + std::to_string(_width) + ".\n";
			throw IndexOutOfBoundsError(s);
		}
		return result;
	}

	bool Grid::isValidCell(int row, int col, bool throwOnFail) const
	{
		bool result = isValidRow(row) && isValidCol(col);
		if (throwOnFail && !result)
		{
			std::string s = "Invalid cell (" + std::to_string(row) + ", " + std::to_string(col) + ") for grid with dimensions (" + std::to_string(_height) + ", " + std::to_string(_width) + ").\n";
			throw IndexOutOfBoundsError(s);
		}
		return result;
	}

	bool Grid::hintsAreConsistent() const
	{
		return sumOfVectorSums(_horizontalHints) == sumOfVectorSums(_verticalHints);
	}

	bool Grid::isSolved() const
	{
		// Simply return false if any row/column doesn't satisfty its corresponding hints.
		for (int i = 0; i < _height; i++)
		{
			if (!cellsSatisfyHints(getRow(i), _horizontalHints[i]))
			{
				return false;
			}
		}

		for (int i = 0; i < _width; i++)
		{
			if (!cellsSatisfyHints(getCol(i), _verticalHints[i]))
			{
				return false;
			}
		}

		return true;
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
			std::string s = "Hints " + vectorToString(hints, ", ", "(", ")") + " require minimum space " + std::to_string(space) + " which exceeds grid width (" + std::to_string(_width) + "), and thus are invalid.\n";
			throw InvalidGridHintsError(s);
		}
		return result;
	}

	bool Grid::areValidColHints(const std::vector<int>& hints, bool throwOnFail) const
	{
		int space = minimumSpaceFromHints(hints);
		bool result = space <= _height;
		if (throwOnFail && !result)
		{
			std::string s = "Hints " + vectorToString(hints, ", ", "(", ")") + " require minimum space " + std::to_string(space) + " which exceeds grid height (" + std::to_string(_height) + "), and thus are invalid.\n";
			throw InvalidGridHintsError(s);
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

	bool operator!=(const Grid& lhs, const Grid& rhs)
	{
		return !(lhs == rhs);
	}
}