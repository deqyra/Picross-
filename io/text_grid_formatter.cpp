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
		_emptyChar(_defaultEmptyChar),
		_crossedChar(_defaultCrossedChar)
	{

	}

	TextGridFormatter::TextGridFormatter(std::string checkedChar, std::string emptyChar, std::string crossedChar) :
		_checkedChar(checkedChar),
		_emptyChar(emptyChar),
		_crossedChar(crossedChar)
	{

	}

	std::string TextGridFormatter::renderGrid(const Grid& grid, bool emptyCrossedCells)
	{
		int width = grid.getWidth();
		int height = grid.getHeight();

		std::string s;
		// First line
		s += renderTopLine(width);

		// All rows and interlines
		for (int i = 0; i < height - 1; i++)
		{
			s += renderRow(grid.getRow(i), emptyCrossedCells);
			s += renderInterline(width);
		}

		// Last row and bottom line
		s += renderRow(grid.getRow(height - 1));
		s += renderBottomLine(width);

		return s;
	}

	std::string TextGridFormatter::renderGridWithHints(const Grid& grid, bool emptyCrossedCells)
	{
		int width = grid.getWidth();
		int height = grid.getHeight();

		int hPadding = IterTools::maxIterableLength(grid.getAllRowHints()) * 2 - 1;
		int vPadding = IterTools::maxIterableLength(grid.getAllColHints()) * 2 - 1;

		// In all blocks remove the final \n to negate useless newline concatenation
		std::string paddingBlock = padBlock(hPadding, vPadding, " ");
		paddingBlock.pop_back();		// Remove final \n
		std::string vHintsStr = renderVerticalHints(grid.getAllColHints());
		vHintsStr.pop_back();			// Remove final \n
		std::string hHintsStr = renderHorizontalHints(grid.getAllRowHints());
		hHintsStr.pop_back();			// Remove final \n
		std::string gridStr = renderGrid(grid, emptyCrossedCells);
		gridStr.pop_back();				// Remove final \n

		std::string s;
		s += StringTools::multilineConcatenation(paddingBlock, vHintsStr);
		s += StringTools::multilineConcatenation(hHintsStr, gridStr);
		return s;
	}

	std::string TextGridFormatter::getCharacter(cell_t cellContent)
	{
		switch(cellContent)
		{
			case CELL_CHECKED:
				return _checkedChar;
				break;
			case CELL_CLEARED:
				return _emptyChar;
				break;
			case CELL_CROSSED:
				return _crossedChar;
				break;
			default:
				// Auto throw if character is not valid
				isValidCellValue(cellContent, true);
				std::string s = "Cell value unhandled by formatter: " + cellValueToString(cellContent) + ".\n";
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
				_emptyChar = newChar;
				break;
			case CELL_CROSSED:
				_crossedChar = newChar;
				break;
			default:
				// Auto throw if character is not valid
				isValidCellValue(cellContent, true);
				std::string s = "Cell value unhandled by formatter: " + cellValueToString(cellContent) + ".\n";
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
				_emptyChar = _defaultEmptyChar;
				break;
			case CELL_CROSSED:
				_crossedChar = _defaultCrossedChar;
				break;
			default:
				// Auto throw if character is not valid
				isValidCellValue(cellContent, true);
				std::string s = "Cell content type unhandled by formatter: " + cellValueToString(cellContent) + ".\n";
				throw UnrecognizedCellValueError(s);
		}
	}

	void TextGridFormatter::resetAllCharacters()
	{
		_checkedChar = _defaultCheckedChar;
		_emptyChar = _defaultEmptyChar;
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
		std::string s;
		s += VERTICAL_CHAR;

		int width = row.size();
		for (int i = 0; i < width; i++)
		{
			std::string contentChar;
			switch (row[i])
			{
				case CELL_CHECKED:
					contentChar = _checkedChar;
					break;
				case CELL_CLEARED:
					contentChar = _emptyChar;
					break;
				case CELL_CROSSED:
					contentChar = _crossedChar;
					if (emptyCrossedCells)
					{
						contentChar = _emptyChar;
					}
					break;
				default:
					contentChar = " ";
			}
			s += contentChar + VERTICAL_CHAR;
		}
		s += '\n';

		return s;
	}

	std::string TextGridFormatter::renderInterline(int width)
	{
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
		int maxHintLength = IterTools::maxIterableLength(hints);
		int strLength = maxHintLength * 2 - 1;

		std::string s;
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			s += pad(strLength, HORIZONTAL_CHAR) + '\n';
	
			int lengthDiff = maxHintLength - it->size();
			if (lengthDiff)
			{
				s += pad(lengthDiff * 2, " ");
			}
	
			s += StringTools::iterableToString(*it) + '\n';
		}
		s += pad(strLength, HORIZONTAL_CHAR) + '\n';

		return s;
	}

	std::string TextGridFormatter::renderVerticalHints(std::vector<std::vector<int>> hints)
	{
		std::vector<std::string> strHints;
		int maxHintLength = IterTools::maxIterableLength(hints);

		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			std::string s;
			int lengthDiff = maxHintLength - it->size();
			s += pad(lengthDiff, " ") + StringTools::iterableToString(*it, "");
			strHints.push_back(s);
		}

		std::string s;
		for (int i = 0; i < maxHintLength; i++)
		{
			for (auto it = strHints.begin(); it != strHints.end(); it++)
			{
				s += VERTICAL_CHAR + it->at(i);
			}
			s += VERTICAL_CHAR + '\n';

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