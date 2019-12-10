#include "grid.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <iostream>

#include "cell_t.hpp"
#include "utility.hpp"
#include "exceptions/invalid_grid_hints_error.hpp"
#include "../tools/exceptions/index_out_of_bounds_error.hpp"
#include "../tools/iterable_tools.hpp"
#include "../tools/exceptions/unmatched_array_size_error.hpp"
#include "../io/text_grid_formatter.hpp"

namespace Picross
{
	Grid::Grid(int width, int height) :
		_width(width),
		_height(height),
		_content(width * height, CELL_CLEARED),
		_rowHints(height, std::vector<int>()),
		_colHints(width, std::vector<int>())
	{

	}

	Grid::Grid(int width, int height, std::vector<std::vector<int>> horizontalHints, std::vector<std::vector<int>> verticalHints) :
		_width(width),
		_height(height),
		_content(width * height, CELL_CLEARED),
		_rowHints(horizontalHints),
		_colHints(verticalHints)
	{
		// Throw and cancel object creation if any hints are invalid.
		// Catch all exceptions and throw a big one with the text of all of them.
		bool exceptionCaught = false;
		std::string exceptionText;

		// Throw if provided hints are a different size from the grid dimensions.
		if (_rowHints.size() != _height)
		{
			std::string str = "Number of provided horizontal hint entries (" + std::to_string(_rowHints.size()) + ") is different from grid height (" + std::to_string(_height) + ").";
			throw InvalidGridHintsError(str);
		}

		if (_colHints.size() != _width)
		{
			std::string str = "Number of provided vertical hints entries (" + std::to_string(_colHints.size()) + ") is different from grid width (" + std::to_string(_width) + ").";
			throw InvalidGridHintsError(str);
		}

		// Check whether all hints are valid. Catch any exception along the way and aggregate all text.
		for (auto it = _rowHints.begin(); it != _rowHints.end(); it++)
		{
			try
			{
				areValidRowHints((*it), true);
			}
			catch(const InvalidGridHintsError& e)
			{
				exceptionCaught = true;
				exceptionText += e.what();
				exceptionText += '\n';
			}
		}

		for (auto it = _colHints.begin(); it != _colHints.end(); it++)
		{
			try
			{
				areValidColHints((*it), true);
			}
			catch(const InvalidGridHintsError& e)
			{
				exceptionCaught = true;
				exceptionText += e.what();
				exceptionText += '\n';
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
		return _rowHints;
	}

	std::vector<int> Grid::getRowHints(int row) const
	{
		// This will throw if the check fails (last parameter).
		isValidRow(row, true);

		return _rowHints[row];
	}

	std::vector<std::vector<int>> Grid::getAllColHints() const
	{
		return _colHints;
	}

	std::vector<int> Grid::getColHints(int col) const
	{
		// This will throw if the check fails (last parameter).
		isValidCol(col, true);

		return _colHints[col];
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

	void Grid::setCellRange(int i0, int in, int j0, int jn, cell_t val)
	{
		// This will throw if the checks fail (last parameter).
		isValidCellValue(val, true);

		// Throw if given range is invalid.
		if (!isValidRow(i0) || !isValidRow(in) || !isValidCol(j0) || !isValidCol(jn))
		{
			std::string s = "Range (" + std::to_string(i0) + ":" + std::to_string(in) + " ; " + std::to_string(j0) + ":" + std::to_string(jn) + ") is out of bound for grid of dimensions (" + std::to_string(_height) + ", " + std::to_string(_width) + ").";
			throw IndexOutOfBoundsError(s.c_str());
		}

		// Swap range extents if wrongly ordered.
		if (i0 > in)
		{
			std::swap(i0, in);
		}
		if (j0 > jn)
		{
			std::swap(j0, jn);
		}

		// Set given value for all cells in range.
		for (int i = i0; i <= in; i++)
		{
			for (int j = j0; j <= jn; j++)
			{
				_content[(i * _width) + j] = val;
			}
		}
	}

	void Grid::setRow(int row, std::vector<cell_t> array)
	{
		// Auto-throw on invalid row index.
		isValidRow(row, true);

		// Check length of provided array.
		if (array.size() != _width)
		{
			std::string s = "setRow: passed array has size " + std::to_string(array.size()) + " but grid has width " + std::to_string(_width) + ".";
			throw UnmatchedArraySizeError(s);
		}

		for (int i = 0; i  < array.size(); i++)
		{
			// Auto-throw on invalid cell value.
			isValidCellValue(array[i], true);
		}

		// Copy all cell values into row.
		for (int i = 0; i < _width; i++)
		{
			setCell(row, i, array[i]);
		}
	}

	void Grid::setCol(int col, std::vector<cell_t> array)
	{
		// Auto-throw on invalid col index.
		isValidCol(col, true);

		// Check length of provided array.
		if (array.size() != _height)
		{
			std::string s = "setCol: passed array has size " + std::to_string(array.size()) + " but grid has height " + std::to_string(_height) + ".";
			throw UnmatchedArraySizeError(s);
		}

		for (int i = 0; i  < array.size(); i++)
		{
			// Auto-throw on invalid cell value.
			isValidCellValue(array[i], true);
		}

		// Copy all cell values into col.
		for (int i = 0; i < _height; i++)
		{
			setCell(i, col, array[i]);
		}
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

		_rowHints[row] = hints;
	}

	void Grid::setColHints(int col, std::vector<int> hints)
	{
		// These checks will throw on fails (last parameter).
		isValidCol(col, true);
		areValidColHints(hints, true);

		_colHints[col] = hints;
	}

	void Grid::setAllRowHints(std::vector<std::vector<int>> hints)
	{
		if (hints.size() != _height)
		{
			std::string s = "Horizontal hints vector size (" + std::to_string(hints.size()) + ") must be the same as grid height (" + std::to_string(_height) + ").";
			throw InvalidGridHintsError(s);
		}

		// Set all hints using member method to check on hint sequence validity.
		for (int i = 0; i < hints.size(); i++)
		{
			setRowHints(i, hints[i]);
		}
	}

	void Grid::setAllColHints(std::vector<std::vector<int>> hints)
	{
		if (hints.size() != _width)
		{
			std::string s = "Vertical hints vector size (" + std::to_string(hints.size()) = ") must be the same as grid width (" + std::to_string(_width) + ").";
			throw InvalidGridHintsError(s);
		}

		// Set all hints using member method to check on hint sequence validity.
		for (int i = 0; i < hints.size(); i++)
		{
			setColHints(i, hints[i]);
		}
	}

	void Grid::setHintsFromState()
	{
		std::vector<std::vector<int>> newRowHints;
		std::vector<std::vector<int>> newColHints;

		// Fill those two new vectors with hints generated from current columns and rows.
		for (int i = 0; i < _height; i++)
		{
			newRowHints.push_back(hintsFromCells(getRow(i)));
		}

		for (int i = 0; i < _width; i++)
		{
			newColHints.push_back(hintsFromCells(getCol(i)));
		}

		// Assign those.
		_rowHints = newRowHints;
		_colHints = newColHints;
	}

	void Grid::clearRowHints()
	{
		// Reset row hints.
		for (int i = 0; i < _height; i++)
		{
			_rowHints[i] = {};
		}
	}

	void Grid::clearColHints()
	{
		// Reset column hints.
		for (int i = 0; i < _width; i++)
		{
			_colHints[i] = {};
		}
	}

	void Grid::clearAllHints()
	{
		// Reset all hints.
		clearRowHints();
		clearColHints();
	}

	bool Grid::isValidRow(int row, bool throwOnFail) const
	{
		bool valid = row >= 0 && row < _height;
		if (throwOnFail && !valid)
		{
			std::string s = "Invalid row " + std::to_string(row) + " for grid with height " + std::to_string(_height) + ".";
			throw IndexOutOfBoundsError(s);
		}
		return valid;
	}

	bool Grid::isValidCol(int col, bool throwOnFail) const
	{
		bool valid = col >= 0 && col < _width;
		if (throwOnFail && !valid)
		{
			std::string s = "Invalid column " + std::to_string(col) + " for grid with width " + std::to_string(_width) + ".";
			throw IndexOutOfBoundsError(s);
		}
		return valid;
	}

	bool Grid::isValidCell(int row, int col, bool throwOnFail) const
	{
		bool valid = isValidRow(row) && isValidCol(col);
		if (throwOnFail && !valid)
		{
			std::string s = "Invalid cell (" + std::to_string(row) + ", " + std::to_string(col) + ") for grid with dimensions (" + std::to_string(_height) + ", " + std::to_string(_width) + ").";
			throw IndexOutOfBoundsError(s);
		}
		return valid;
	}

	bool Grid::hintsAreConsistent() const
	{
		// Hints are consistent if the sum of horinzontal hints equals the sum of vertical hints. 
		return IterTools::sum2NestedIterables<int>(_rowHints) == IterTools::sum2NestedIterables<int>(_colHints);
	}

	bool Grid::isSolved() const
	{
		// Simply return false if any row/column doesn't satisfty its corresponding hints.
		for (int i = 0; i < _height; i++)
		{
			if (!cellsSatisfyHints(getRow(i), _rowHints[i]))
			{
				return false;
			}
		}

		for (int j = 0; j < _width; j++)
		{
			if (!cellsSatisfyHints(getCol(j), _colHints[j]))
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
		// Arguments here are iterators (pointers really) the begining of the array and its element count.
        int maxIndex = IterTools::indexOfMaxElement(counts, CELL_T_VALUE_COUNT);
        return CELL_T_ORDERED_VALUES[maxIndex];
    }

	bool Grid::areValidRowHints(const std::vector<int>& hints, bool throwOnFail) const
	{
		// Check whether provided hints fit in a row of the grid.
		int space = minimumSpaceFromHints(hints);
		bool valid = space <= _width;
		if (throwOnFail && !valid)
		{
			std::string s = "Hints " + StringTools::iterableToString(hints, ", ", "(", ")") + " require minimum space " + std::to_string(space) + " which exceeds grid width (" + std::to_string(_width) + "), and thus are invalid.";
			throw InvalidGridHintsError(s);
		}
		return valid;
	}

	bool Grid::areValidColHints(const std::vector<int>& hints, bool throwOnFail) const
	{
		// Check whether provided hints fit in a column of the grid.
		int space = minimumSpaceFromHints(hints);
		bool valid = space <= _height;
		if (throwOnFail && !valid)
		{
			std::string s = "Hints " + StringTools::iterableToString(hints, ", ", "(", ")") + " require minimum space " + std::to_string(space) + " which exceeds grid height (" + std::to_string(_height) + "), and thus are invalid.";
			throw InvalidGridHintsError(s);
		}
		return valid;
	}

	bool operator==(const Grid& lhs, const Grid& rhs)
	{
		// Return false if any member is not equal in both grids.
		if (lhs._width != rhs._width) return false;
		if (lhs._height != rhs._height) return false;
		if (lhs._rowHints != rhs._rowHints) return false;
		if (lhs._colHints != rhs._colHints) return false;
		if (lhs._content != rhs._content) return false;

		return true;
	}

	bool operator!=(const Grid& lhs, const Grid& rhs)
	{
		return !(lhs == rhs);
	}
}

std::ostream& operator<<(std::ostream& os, Picross::Grid const& grid)
{
    // Print the input grid with hints.
    Picross::TextGridFormatter txt = Picross::TextGridFormatter();
    os << txt.renderGridWithHints(grid);
    return os;
}
