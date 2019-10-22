#ifndef GRID_HPP
#define GRID_HPP

#include <vector>
#include <string>

#include "cell_t.hpp"

namespace Picross
{
	class Grid
	{
		private:
			int _width;
			int _height;
			std::vector<cell_t> _content;			// 1D-array containing the "unfolded" grid, row-major indexed.
			std::vector<std::vector<int>> _horizontalHints;
			std::vector<std::vector<int>> _verticalHints;

		public:
			Grid(int width, int height);
			Grid(int width, int height, std::vector<std::vector<int>> horizontalHints, std::vector<std::vector<int>> verticalHints);
			~Grid();

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
			void checkCell(int row, int col);
			void crossCell(int row, int col);
			void clearCell(int row, int col);

			// Tip modification methods.
			void setRowHints(int row, std::vector<int> hints);
			void setColHints(int col, std::vector<int> hints);
			void setAllRowHints(std::vector<std::vector<int>> hints);
			void setAllColHints(std::vector<std::vector<int>> hints);

			// Useful coordinate checks.
			bool isValidRow(int row, bool throwOnFail = false) const;
			bool isValidCol(int col, bool throwOnFail = false) const;
			bool isValidCell(int row, int col, bool throwOnFail = false) const;

			// Useful tip-related checks and functions.
			bool areValidRowHints(const std::vector<int>& hints, bool throwOnFail = false) const;
			bool areValidColHints(const std::vector<int>& hints, bool throwOnFail = false) const;
	};
}

#endif//GRID_HPP