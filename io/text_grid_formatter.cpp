#include "text_grid_formatter.hpp"
#include "../core/utility.hpp"
#include "../core/exceptions/unrecognized_cell_value_error.hpp"
#include "../tools/string_tools.hpp"
#include "../tools/iterable_tools.hpp"

#include <iostream>
#include <stdexcept>

namespace Picross
{
	TextGridFormatter::TextGridFormatter() :
		_checkedChar(_defaultCheckedChar),
		_clearedChar(_defaultClearedChar),
		_crossedChar(_defaultCrossedChar)
	{

	}

	TextGridFormatter::TextGridFormatter(std::string checkedChar, std::string emptyChar, std::string crossedChar) :
		_checkedChar(checkedChar),
		_clearedChar(emptyChar),
		_crossedChar(crossedChar)
	{

	}

	std::string TextGridFormatter::renderGrid(const Grid& grid, bool emptyCrossedCells)
	{
		int width = grid.getWidth();
		int height = grid.getHeight();

		std::string s;
		// First line.
		s += renderTopLine(width);

		// All rows and interlines (except the last row).
		for (int i = 0; i < height - 1; i++)
		{
			s += renderRow(grid.getRow(i), emptyCrossedCells);
			s += renderInterline(width);
		}

		// Last row and bottom line.
		s += renderRow(grid.getRow(height - 1));
		s += renderBottomLine(width);

		return s;
	}

	std::string TextGridFormatter::renderGridWithHints(const Grid& grid, bool emptyCrossedCells)
	{
		// Separately render 4 elements:
		// - a padding block (empty space top left of the displayed grid),
		// - the vertical hints of the grid,
		// - the horizontal hints of the grid,
		// - the grid itself.
		// Then combine all of those to get the final render.

		int width = grid.getWidth();
		int height = grid.getHeight();

		// Calculate necessary character paddings.
		int hPadding = IterTools::maxIterableLength(grid.getAllRowHints()) * 2 - 1;
		int vPadding = IterTools::maxIterableLength(grid.getAllColHints()) * 2 - 1;

		// Exceptional case: either directional hints are ALL empty, resulting in padding -1. Set it back to 0.
		if (hPadding == -1) hPadding = 0;
		if (vPadding == -1) vPadding = 0;

		// Separately render all four aforementioned blocks.
		std::string paddingBlock = padBlock(hPadding, vPadding, " ");
		std::string vHintsStr = renderVerticalHints(grid.getAllColHints());
		std::string hHintsStr = renderHorizontalHints(grid.getAllRowHints());
		std::string gridStr = renderGrid(grid, emptyCrossedCells);

		// In all blocks (if appropriate), remove the final \n to negate useless newline concatenation.
		if (paddingBlock.length()) paddingBlock.pop_back();
		if (vHintsStr.length()) vHintsStr.pop_back();
		if (hHintsStr.length()) hHintsStr.pop_back();
		gridStr.pop_back();

		// Combine all blocks to get the final render.
		std::string s;
		s += StringTools::multilineConcatenation(paddingBlock, vHintsStr);
		s += StringTools::multilineConcatenation(hHintsStr, gridStr);
		return s;
	}

	std::string TextGridFormatter::getCharacter(cell_t cellContent)
	{
		// Return the character used to represent a particular state in a cell.
		switch(cellContent)
		{
			case CELL_CHECKED:
				return _checkedChar;
				break;
			case CELL_CLEARED:
				return _clearedChar;
				break;
			case CELL_CROSSED:
				return _crossedChar;
				break;
			default:
				// Auto throw if character is not valid.
				isValidCellValue(cellContent, true);
				// Otherwise the cell state might just not be recognized.
				std::string s = "TextGridFormatter: unhandled cell value: " + cellValueToString(cellContent) + ".\n";
				throw UnrecognizedCellValueError(s);
		}
	}

	void TextGridFormatter::setCharacter(cell_t cellContent, std::string newChar)
	{
		if (newChar.length() != 1)
		{
			std::string s = "TextGridFormatter replacement characters must be 1 in length, cannot accept \"" + newChar + "\".\n";
			throw UnrecognizedCellValueError(s);
		}
		
		switch(cellContent)
		{
			case CELL_CHECKED:
				_checkedChar = newChar;
				break;
			case CELL_CLEARED:
				_clearedChar = newChar;
				break;
			case CELL_CROSSED:
				_crossedChar = newChar;
				break;
			default:
				// Auto throw if character is not valid.
				isValidCellValue(cellContent, true);
				// Otherwise the cell state might just not be recognized.
				std::string s = "TextGridFormatter: unhandled cell value: " + cellValueToString(cellContent) + ".\n";
				throw UnrecognizedCellValueError(s);
		}
	}

	void TextGridFormatter::resetCharacter(cell_t cellContent)
	{
		switch(cellContent)
		{
			case CELL_CHECKED:
				_checkedChar = _defaultCheckedChar;
				break;
			case CELL_CLEARED:
				_clearedChar = _defaultClearedChar;
				break;
			case CELL_CROSSED:
				_crossedChar = _defaultCrossedChar;
				break;
			default:
				// Auto throw if character is not valid.
				isValidCellValue(cellContent, true);
				// Otherwise the cell state might just not be recognized.
				std::string s = "TextGridFormatter: unhandled cell value: " + cellValueToString(cellContent) + ".\n";
				throw UnrecognizedCellValueError(s);
		}
	}

	void TextGridFormatter::resetAllCharacters()
	{
		_checkedChar = _defaultCheckedChar;
		_clearedChar = _defaultClearedChar;
		_crossedChar = _defaultCrossedChar;
	}

	std::string TextGridFormatter::pad(int length, std::string padString)
	{
		std::string s;
		for (int i = 0; i < length; i++)
		{
			s += padString;
		}
		return s;
	}

	std::string TextGridFormatter::padBlock(int width, int height, std::string padString)
	{
		std::string s;
		for (int i = 0; i < height; i++)
		{
			s += pad(width, padString) + '\n';
		}
		return s;
	}

	std::string TextGridFormatter::renderTopLine(int width)
	{
		// The top line looks like this:
		// ╔═╦═╦═╦═╦═ ... ╦═╦═╦═╦═╦═╗

		std::string s;
		s += TOP_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += HORIZONTAL_CHAR + TOP_CROSS_CHAR;
		}
		s += HORIZONTAL_CHAR + TOP_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderRow(const std::vector<cell_t>& row, bool emptyCrossedCells)
	{
		// A row looks like this:
		// ║■║ ║■║ ║ ║ ... ║×║×║×║ ║

		std::string s;
		s += VERTICAL_CHAR;

		int width = row.size();
		for (int i = 0; i < width; i++)
		{
			std::string contentChar = getCharacter(row[i]);
			if (emptyCrossedCells)
			{
				contentChar = getCharacter(CELL_CLEARED);
			}
			s += contentChar + VERTICAL_CHAR;
		}
		s += '\n';

		return s;
	}

	std::string TextGridFormatter::renderInterline(int width)
	{
		// An interline looks like this:
		// ╠═╬═╬═╬═╬═╬═ ... ╬═╬═╬═╬═╣

		std::string s;
		s += LEFT_CROSS_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += HORIZONTAL_CHAR + MIDDLE_CROSS_CHAR;
		}
		s += HORIZONTAL_CHAR + RIGHT_CROSS_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderBottomLine(int width)
	{
		// The bottom line looks like this:
		// ╚═╩═╩═╩═╩═ ... ╩═╩═╩═╩═╩═╝
		
		std::string s;
		s += BOTTOM_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += HORIZONTAL_CHAR + BOTTOM_CROSS_CHAR;
		}
		s += HORIZONTAL_CHAR + BOTTOM_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderHorizontalHints(std::vector<std::vector<int>> hints)
	{
		// The max hint sequence length is needed to pad shorter hint sequence, in order for all to be the same size.
		int maxHintLength = IterTools::maxIterableLength(hints);
		// As hints are separated by a space, compute the needed string length.
		int strLength = maxHintLength * 2 - 1;
		// Exceptional case: hints are ALL empty, resulting in length -1. Set it back to 0.
		if (strLength == -1) strLength = 0;

		std::string s;
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			// Generate a separator string like:
			// ══════════ ... ═════════
			s += pad(strLength, HORIZONTAL_CHAR) + '\n';

			// Compute how many spaces need to be inserted to properly pad the hint sequence.	
			int lengthDiff = maxHintLength - it->size();
			// Generate padding.
			if (lengthDiff)
			{
				s += pad(lengthDiff * 2, " ");
			}
	
			// Concatenate contents of the hint sequence.
			s += StringTools::iterableToString(*it) + '\n';
		}
		// Generate bottom separator string.
		s += pad(strLength, HORIZONTAL_CHAR) + '\n';

		return s;
	}

	std::string TextGridFormatter::renderVerticalHints(std::vector<std::vector<int>> hints)
	{
		// As these hints need to be rendered vertically, the rendering becomes non-trivial.
		// Step 1: stringify all hint sequences, padding where necessary.
		// Step 2: 

		// The max hint sequence length is needed to pad shorter hint sequences, in order for all to be the same size.
		int maxHintLength = IterTools::maxIterableLength(hints);

		std::vector<std::string> strHints;
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			std::string s;
			// Make all hints padded string of the same length.
			int lengthDiff = maxHintLength - it->size();
			s += pad(lengthDiff, " ") + StringTools::iterableToString(*it, "");
			strHints.push_back(s);
		}
		// At this point, strHints contains stringified hints like so:
		// " 341"
		// "2221"
		// "  63"
		// Considering inserted spaces are "empty hints", all the hint sequences are the same size.

		std::string s;
		// "Verticalize" all previously stringified hints, inserting vertical separators and padding where needed.
		for (int i = 0; i < maxHintLength; i++)
		{
			// Normal line: print all hints separated by the vertical separator.
			// ║1║2║3║4║5║6║7║8║9║ ║
			for (auto it = strHints.begin(); it != strHints.end(); it++)
			{
				s += VERTICAL_CHAR + it->at(i);
			}
			s += VERTICAL_CHAR + '\n';

			// Separator line: print spaces separated by the vertical separator.
   			// ║ ║ ║ ║ ║ ║ ║ ║ ║ ║ ║
			if (i != maxHintLength - 1)
			{
				for (int j = 0; j < strHints.size(); j++)
				{
					s += VERTICAL_CHAR + " ";
				}
				s += VERTICAL_CHAR + '\n';
			}
		}

		return s;
	}
}