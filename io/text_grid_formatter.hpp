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
			std::string renderGrid(const Grid& grid, bool emptyCrossedCells = false, int cellWidth = 1);
			std::string renderGridWithHints(const Grid& grid, bool emptyCrossedCells = false);
			
		// Cell character customization
			std::string getCharacter(cell_t cellContent);
			void setCharacter(cell_t cellContent, std::string newChar);
			void resetCharacter(cell_t cellContent);
			void resetAllCharacters();

		private:	// Private methods
			// Find the minimum char width needed to render given vertical hints.
			static int findCharWidthFromColHintSequences(const std::vector<std::vector<int>>& hintVectors);

		// Grid rendering tools
			// Renders the provided grid row into a string.
			std::string renderRow(const std::vector<cell_t>& row, bool emptyCrossedCells, int cellWidth);
			// Renders the top line of a grid into a string.
			static std::string renderTopLine(int width, int cellWidth);
			// Renders an interline of a grid into a string.
			static std::string renderInterline(int width, int cellWidth);
			// Renders the bottom line of a grid into a string.
			static std::string renderBottomLine(int width, int cellWidth);

		// Hint rendering tools
			// Renders the horizontal hints of a grid into a string.
			static std::string renderStringifiedRowHints(const std::vector<std::string>& stringifiedHintVectors);
			// Renders the vertical hints of a grid into a string.
			static std::string renderStringifiedColHintSequenceVector(const std::vector<std::vector<std::string>>& stringifiedHintVectors);

		// Padding tools
			// Returns a string containing `length` times `padString`.
			static std::string pad(int length, const std::string& padString);
			// Returns a string containing `height` lines, each containing `width` times `padString`.
			static std::string padBlock(int width, int height, const std::string& padString);

		// Hint stringifying tools
			// Turns a vector of hint sequences into a vector of stringified hint sequences.
			static std::vector<std::string> stringifyRowHintSequences(const std::vector<std::vector<int>>& hintVectors);
			// Turn a vector of int hint sequences into a vector of individually stringified and padded hints.
			static std::vector<std::vector<std::string>> stringifyColHintSequences(const std::vector<std::vector<int>>& hintVectors, int charWidth);
			// Given a hint sequence, individually stringify and pad every hint, and prepend empty strings to the final vector so its size matches the requested max hint sequence length.
			static std::vector<std::string> stringifyColHintsInVector(const std::vector<int>& hints, int charWidth, int maxHintSequenceLength);
			// Stringify a single hint and pad it so the final string length matches the requested char width.
			static std::string stringifyAndPadHint(const int& hint, int charWidth);
	};
}

#endif//IO__TEXT_GRID_FORMATTER_HPP