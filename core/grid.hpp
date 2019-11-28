#ifndef CORE__GRID_HPP
#define CORE__GRID_HPP

#include <vector>
#include <string>

#include "cell_t.hpp"

namespace Picross
{
	inline static const int GRID_MAX_SIZE = 20;
	
	class Grid
	{
		private:	// Attributes
			int _width;
			int _height;
			std::vector<cell_t> _content;			// 1D-array containing the "unfolded" grid, row-major indexed.
			std::vector<std::vector<int>> _rowHints;
			std::vector<std::vector<int>> _colHints;

		public:		// Public methods
			Grid(int width, int height);
			Grid(int width, int height, std::vector<std::vector<int>> horizontalHints, std::vector<std::vector<int>> verticalHints);

			int getWidth() const;
			int getHeight() const;

		// These return COPIES only. Modifications to the grid content and hints to be made through the appropriate methods.
			std::vector<cell_t> getRow(int row) const;
			std::vector<cell_t> getCol(int col) const;

			std::vector<std::vector<int>> getAllRowHints() const;
			std::vector<int> getRowHints(int row) const;
			std::vector<std::vector<int>> getAllColHints() const;
			std::vector<int> getColHints(int col) const;

		// Cell modification methods.
			cell_t getCell(int row, int col) const;
			void setCell(int row, int col, cell_t val);
			void setCellRange(int i0, int in, int j0, int jn, cell_t val);
			void checkCell(int row, int col);
			void crossCell(int row, int col);
			void clearCell(int row, int col);

		// Hint modification methods.
			void setRowHints(int row, std::vector<int> hints);
			void setColHints(int col, std::vector<int> hints);
			void setAllRowHints(std::vector<std::vector<int>> hints);
			void setAllColHints(std::vector<std::vector<int>> hints);
			void setHintsFromState();
			void clearRowHints();
			void clearColHints();
			void clearAllHints();

		// Useful checks.
			bool isValidRow(int row, bool throwOnFail = false) const;
			bool isValidCol(int col, bool throwOnFail = false) const;
			bool isValidCell(int row, int col, bool throwOnFail = false) const;
			bool hintsAreConsistent() const;
			bool isSolved() const;
		
		// Return the cell value which is most present within a grid.
			cell_t mostPresentState() const;

		// Useful hint-related checks and functions.
			bool areValidRowHints(const std::vector<int>& hints, bool throwOnFail = false) const;
			bool areValidColHints(const std::vector<int>& hints, bool throwOnFail = false) const;

			friend bool operator==(const Grid& lhs, const Grid& rhs);
			friend bool operator!=(const Grid& lhs, const Grid& rhs);
	};
}

#endif//CORE__GRID_HPP