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
	const std::function<bool(const cell_t&)> Grid::_throwingCellChecker = [] (const cell_t& val) {
		// Returns true (valid) or throws (invalid).
		return isValidCellValue(val, true);
	};

	Grid::Grid(int width, int height) :
		_width(width),
		_height(height),
		_innerGrid(height, width, CELL_CLEARED),
		_rowHints(height, std::vector<int>()),
		_colHints(width, std::vector<int>())
	{

	}

	Grid::Grid(int width, int height, std::vector<std::vector<int>> horizontalHints, std::vector<std::vector<int>> verticalHints) :
		_width(width),
		_height(height),
		_innerGrid(height, width, CELL_CLEARED),
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
		return _innerGrid.getRow(row);
	}

	std::vector<cell_t> Grid::getCol(int col) const
	{
		return _innerGrid.getCol(col);
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
		return _innerGrid.getCell(row, col);
	}

	void Grid::setCell(int row, int col, cell_t val)
	{
		_innerGrid.setCell(row, col, val, _throwingCellChecker);
	}

	void Grid::setCellRange(int i0, int in, int j0, int jn, cell_t val)
	{
		_innerGrid.setCellRange(i0, in, j0, jn, val, _throwingCellChecker);
	}

	void Grid::setRow(int row, std::vector<cell_t> array)
	{
		_innerGrid.setRow(row, array, _throwingCellChecker);
	}

	void Grid::setCol(int col, std::vector<cell_t> array)
	{
		_innerGrid.setCol(col, array, _throwingCellChecker);
	}

	void Grid::checkCell(int row, int col)
	{
		_innerGrid.setCell(row, col, CELL_CHECKED, _throwingCellChecker);
	}

	void Grid::crossCell(int row, int col)
	{
		_innerGrid.setCell(row, col, CELL_CROSSED, _throwingCellChecker);
	}

	void Grid::clearCell(int row, int col)
	{
		_innerGrid.setCell(row, col, CELL_CLEARED, _throwingCellChecker);
	}

	void Grid::merge(const Grid& other, int mergingPolicy)
	{
		if ((other._height != _height) || (other._width != _width))
		{
			std::string s = "Grid of dimensions (" + std::to_string(other._height) + ", " + std::to_string(other._width) + ") cannot be merged into grid of dimension (" + std::to_string(_height) + ", " + std::to_string(_width) + ").";
			throw UnmatchedArraySizeError(s);
		}

		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				// May throw depending on the merging policy.
				cell_t newValue = mergeSingleCell(getCell(i, j), other.getCell(i, j), mergingPolicy);
				setCell(i, j, newValue);
			}
		}
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
		return _innerGrid.isValidRow(row, throwOnFail);
	}

	bool Grid::isValidCol(int col, bool throwOnFail) const
	{
		return _innerGrid.isValidCol(col, throwOnFail);
	}

	bool Grid::isValidCell(int row, int col, bool throwOnFail) const
	{
		return _innerGrid.isValidCell(row, col, throwOnFail);
	}

	bool Grid::hintsAreConsistent() const
	{
		// Hints are consistent if the sum of horinzontal hints equals the sum of vertical hints. 
		return rowHintSum() == colHintSum();
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

	int Grid::rowHintSum() const
	{
		return IterTools::sum2NestedIterables<int>(_rowHints);
	}

	int Grid::colHintSum() const
	{
		return IterTools::sum2NestedIterables<int>(_colHints);
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
		if (lhs._innerGrid != rhs._innerGrid) return false;

		return true;
	}

	bool operator!=(const Grid& lhs, const Grid& rhs)
	{
		return !(lhs == rhs);
	}
} // namespace Picross

std::ostream& operator<<(std::ostream& os, Picross::Grid const& grid)
{
    // Print the input grid with hints.
    Picross::TextGridFormatter txt = Picross::TextGridFormatter();
    os << txt.renderGridWithHints(grid);
    return os;
}
