#ifndef TEXT_GRID_FORMATTER_HPP
#define TEXT_GRID_FORMATTER_HPP

#include <string>
#include <vector>

// Grid display characters
#define TOP_LEFT_CHAR "╔"
#define TOP_RIGHT_CHAR "╗"
#define BOTTOM_LEFT_CHAR "╚"
#define BOTTOM_RIGHT_CHAR "╝"
#define LEFT_CROSS_CHAR "╠"
#define RIGHT_CROSS_CHAR "╣"
#define TOP_CROSS_CHAR "╦"
#define BOTTOM_CROSS_CHAR "╩"
#define MIDDLE_CROSS_CHAR "╬"
#define VERTICAL_CHAR "║"
#define HORIZONTAL_CHAR "═"

#include "../core/cell_t.hpp"
#include "../core/grid.hpp"

namespace Picross
{
	class TextGridFormatter
	{
		private:
			// Cell display characters
			inline static const std::string _defaultCheckedChar = "■";
			inline static const std::string _defaultEmptyChar = " ";
			inline static const std::string _defaultCrossedChar = "×";

			std::string _checkedChar;
			std::string _emptyChar;
			std::string _crossedChar;

			// Render-related utilities
			std::string pad(int length, std::string padString);
			std::string padBlock(int width, int height, std::string padString);

			std::string renderTopLine(int width);
			std::string renderRow(const std::vector<cell_t> &row, bool emptyCrossedCells = false);
			std::string renderInterline(int width);
			std::string renderBottomLine(int width);

			std::string renderHorizontalHints(std::vector<std::vector<int>> hints);
			std::string renderVerticalHints(std::vector<std::vector<int>> hints);

		public:
			TextGridFormatter();
			TextGridFormatter(std::string checkedChar, std::string emptyChar, std::string crossedChar);

			// Render
			std::string renderGrid(const Grid &grid, bool emptyCrossedCells = false);
			std::string renderGridWithHints(const Grid &grid, bool emptyCrossedCells = false);
			
			// Cell character customization
			std::string getCharacter(cell_t cellContent);
			void setCharacter(cell_t cellContent, std::string newChar);
			void resetCharacter(cell_t cellContent);
			void resetAllCharacters();
	};
}

#endif//TEXT_GRID_FORMATTER_HPP