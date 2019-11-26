#ifndef IO__TEXT_GRID_FORMATTER_HPP
#define IO__TEXT_GRID_FORMATTER_HPP

#include <string>
#include <vector>

// Grid display characters
#define TOP_LEFT_CHAR (std::string)"╔"
#define TOP_RIGHT_CHAR (std::string)"╗"
#define BOTTOM_LEFT_CHAR (std::string)"╚"
#define BOTTOM_RIGHT_CHAR (std::string)"╝"
#define LEFT_CROSS_CHAR (std::string)"╠"
#define RIGHT_CROSS_CHAR (std::string)"╣"
#define TOP_CROSS_CHAR (std::string)"╦"
#define BOTTOM_CROSS_CHAR (std::string)"╩"
#define MIDDLE_CROSS_CHAR (std::string)"╬"
#define VERTICAL_CHAR (std::string)"║"
#define HORIZONTAL_CHAR (std::string)"═"

#include "../core/cell_t.hpp"
#include "../core/grid.hpp"

namespace Picross
{
	class TextGridFormatter
	{
		private:	// Attributes
		// Cell display characters
			inline static const std::string _defaultCheckedChar = "■";
			inline static const std::string _defaultClearedChar = " ";
			inline static const std::string _defaultCrossedChar = "×";

			std::string _checkedChar;
			std::string _clearedChar;
			std::string _crossedChar;

		public:		// Public methods
			TextGridFormatter();
			TextGridFormatter(std::string checkedChar, std::string emptyChar, std::string crossedChar);

		// Rendering
			std::string renderGrid(const Grid& grid, bool emptyCrossedCells = false);
			std::string renderGridWithHints(const Grid& grid, bool emptyCrossedCells = false);
			
		// Cell character customization
			std::string getCharacter(cell_t cellContent);
			void setCharacter(cell_t cellContent, std::string newChar);
			void resetCharacter(cell_t cellContent);
			void resetAllCharacters();

		private:	// Private methods
		// Render-related utilities
			// Returns a string containing `length` times `padString`.
			std::string pad(int length, std::string padString);
			// Returns a string containing `height` lines, each containing `width` times `padString`.
			std::string padBlock(int width, int height, std::string padString);

			// Renders into a string the top line of a displayed grid.
			std::string renderTopLine(int width);
			// Renders into a string the provided grid row.
			std::string renderRow(const std::vector<cell_t>& row, bool emptyCrossedCells = false);
			// Renders into a string an interline of a displayed grid.
			std::string renderInterline(int width);
			// Renders into a string the bottom line of a displayed grid.
			std::string renderBottomLine(int width);

			// Renders into a string the horizontal hints of a displayed grid.
			std::string renderHorizontalHints(std::vector<std::vector<int>> hints);
			// Renders into a string the vertical hints of a displayed grid.
			std::string renderVerticalHints(std::vector<std::vector<int>> hints);
	};
}

#endif//IO__TEXT_GRID_FORMATTER_HPP