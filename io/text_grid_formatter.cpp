#include "text_grid_formatter.hpp"
#include "../core/utility.hpp"
#include "../core/exceptions/unrecognized_cell_value_error.hpp"
#include "../tools/string_tools.hpp"
#include "../tools/iterable_tools.hpp"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

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

	std::string TextGridFormatter::renderGrid(const Grid& grid, bool emptyCrossedCells, int cellWidth)
	{
		int width = grid.getWidth();
		int height = grid.getHeight();

		std::string s;
		// First line.
		s += renderTopLine(width, cellWidth);

		// All rows and interlines (except the last row).
		for (int i = 0; i < height - 1; i++)
		{
			s += renderRow(grid.getRow(i), emptyCrossedCells, cellWidth);
			s += renderInterline(width, cellWidth);
		}

		// Last row and bottom line.
		s += renderRow(grid.getRow(height - 1), emptyCrossedCells, cellWidth);
		s += renderBottomLine(width, cellWidth);

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

		// Calculate necessary character paddings (to render a pad block, top left of the final render).
		int hPadding = IterTools::maxIterableLength(grid.getAllRowHints()) * 2 - 1;
		int vPadding = IterTools::maxIterableLength(grid.getAllColHints()) * 2 - 1;

		// Exceptional case: either directional hints are ALL empty, resulting in padding -1. Set it back to 0.
		if (hPadding == -1) hPadding = 0;
		if (vPadding == -1) vPadding = 0;

		// Lambda which takes in a vector of ints and returns the max int from it.
		auto lambda_maxElement = [] (const std::vector<int>& vec) -> int
		{
			// Find the max element.
			auto it = std::max_element(vec.begin(), vec.end());
			// Return it (or 0 if the vector was empty).
			return it != vec.end() ? (*it) : 0;
		};

		// Retrieve the largest hint in each column of the grid.
		std::vector<int> maxColHints = std::vector<int>();
		maxColHints.reserve(grid.getWidth());
		auto allColHints = grid.getAllColHints();
		std::transform(allColHints.begin(), allColHints.end(), maxColHints.begin(), lambda_maxElement);
		// Now find the max hint from all the max col hints.
		int maxHint = *(std::max_element(maxColHints.begin(), maxColHints.end()));

		// Given the biggest column hint in all the grid, find how much char width is needed to display it.
		// This char width will be passed to rendering subroutines to adapt display.
		int charWidth = floor(log10(maxHint)) + 1;

		// Separately render all four aforementioned blocks.
		std::string paddingBlock = padBlock(hPadding, vPadding, " ");
		std::string vHintsStr = renderVerticalHints(grid.getAllColHints(), charWidth);
		std::string hHintsStr = renderHorizontalHints(grid.getAllRowHints());
		std::string gridStr = renderGrid(grid, emptyCrossedCells, charWidth);

		// In all blocks (if appropriate), remove the final \n to negate useless newline concatenation.
		if (paddingBlock.length()) paddingBlock.pop_back();
		if (vHintsStr.length()) vHintsStr.pop_back();
		if (hHintsStr.length()) hHintsStr.pop_back();
		gridStr.pop_back();

		// Combine all blocks to get the final render.
		std::string s;
		if (vPadding)
		{
			// Only use the top blocks if non-empty, otherwise a useless '\n' is inserted.
			s += StringTools::multilineConcatenation(paddingBlock, vHintsStr);
		}
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

	std::string TextGridFormatter::pad(int length, const std::string& padString)
	{
		std::string s;
		for (int i = 0; i < length; i++)
		{
			s += padString;
		}
		return s;
	}

	std::string TextGridFormatter::padBlock(int width, int height, const std::string& padString)
	{
		std::string s;
		for (int i = 0; i < height; i++)
		{
			s += pad(width, padString) + '\n';
		}
		return s;
	}

	std::string TextGridFormatter::renderTopLine(int width, int cellWidth)
	{
		// The top line looks like this:
		// ╔═╦═╦═╦═╦═ ... ╦═╦═╦═╦═╦═╗

		std::string s;
		s += TOP_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += pad(cellWidth, HORIZONTAL_CHAR) + TOP_CROSS_CHAR;
		}
		s += pad(cellWidth, HORIZONTAL_CHAR) + TOP_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderRow(const std::vector<cell_t>& row, bool emptyCrossedCells, int cellWidth)
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
			s += pad(cellWidth, contentChar) + VERTICAL_CHAR;
		}
		s += '\n';

		return s;
	}

	std::string TextGridFormatter::renderInterline(int width, int cellWidth)
	{
		// An interline looks like this:
		// ╠═╬═╬═╬═╬═╬═ ... ╬═╬═╬═╬═╣

		std::string s;
		s += LEFT_CROSS_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += pad(cellWidth, HORIZONTAL_CHAR) + MIDDLE_CROSS_CHAR;
		}
		s += pad(cellWidth, HORIZONTAL_CHAR) + RIGHT_CROSS_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderBottomLine(int width, int cellWidth)
	{
		// The bottom line looks like this:
		// ╚═╩═╩═╩═╩═ ... ╩═╩═╩═╩═╩═╝
		
		std::string s;
		s += BOTTOM_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += pad(cellWidth, HORIZONTAL_CHAR) + BOTTOM_CROSS_CHAR;
		}
		s += pad(cellWidth, HORIZONTAL_CHAR) + BOTTOM_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderHorizontalHints(const std::vector<std::vector<int>>& hintVectors)
	{
		std::vector<std::string> stringifiedHints;
		int maxStringLength = 0;

		for (auto it = hintVectors.begin(); it != hintVectors.end(); it++)
		{
			std::string stringifiedVector = StringTools::iterableToString(*it);
			if (stringifiedVector.length() > maxStringLength)
			{
				maxStringLength = stringifiedVector.length();
			}
			stringifiedHints.push_back(stringifiedVector);
		}

		std::string s;
		for (auto it = stringifiedHints.begin(); it != stringifiedHints.end(); it++)
		{
			// Generate a separator string like:
			// ══════════ ... ═════════
			s += pad(maxStringLength, HORIZONTAL_CHAR) + '\n';

			// Concatenate the padded hints.
			int padLength = maxStringLength - it->length();
			s += pad(padLength, " ") + *it + '\n';
		}
		// Generate bottom separator string.
		s += pad(maxStringLength, HORIZONTAL_CHAR) + '\n';

		return s;
	}

	std::string TextGridFormatter::renderVerticalHints(const std::vector<std::vector<int>>& hintVectors, int charWidth)
	{
		int maxHintSequenceLength = IterTools::maxIterableLength(hintVectors);

		// Lambda which turns an int hint into a space-padded stringified hint.
		auto lambda_stringifyAndPadHint = [charWidth] (const int& hint) -> std::string
		{
			std::string s = std::to_string(hint);
			int padLength = s.length() - charWidth;

			if (padLength > 0)
			{
				s = pad(padLength, " ") + s;
			}

			return s;
		};

		// Lambda which turns a vector of int hints into a vector of space-padded stringified hints.
		auto lambda_intHintVectorToStringHintVector = [lambda_stringifyAndPadHint, maxHintSequenceLength, charWidth] (const std::vector<int>& hints) -> std::vector<std::string>
		{
			// Allocate space for string hints.
			std::vector<std::string> res;
			res.reserve(maxHintSequenceLength);

			// Calculate how many empty strings need to be generated and prepended to the final vector.
			// This is so the hints, once rendered, will all be the same size vertically, like so:
			// ║2║ ║ ║ ║1║
   			// ║ ║ ║ ║ ║ ║
   			// ║1║ ║4║ ║5║
			int hintSequenceLengthDiff = maxHintSequenceLength - hints.size();

			// Generate empty strings and prepend them to the final vector.
			for (int i = 0; i < hintSequenceLengthDiff; i++)
			{
				res.push_back(pad(charWidth, " "));
			}

			// Transform int hints into space-padded string hints and append them to the final vector.
			std::transform(hints.begin(), hints.end(), res.begin() + hintSequenceLengthDiff, lambda_stringifyAndPadHint);
			return res;
		};

		// Allocate space for stringified hint vectors.
		std::vector<std::vector<std::string>> stringifiedHints = std::vector<std::vector<std::string>>();
		stringifiedHints.reserve(hintVectors.size());
		// Transform int hint vectors into padded string hint vectors.
		std::transform(hintVectors.begin(), hintVectors.end(), stringifiedHints.begin(), lambda_intHintVectorToStringHintVector);

		std::string s;
		// For every row of hints across hint vectors...
		for (int i = 0; i < maxHintSequenceLength; i++)
		{
			// Render the actual row across the vectors, like so:
			// ║2║ ║ ║ ║1║
			for (auto it = stringifiedHints.begin(); it != stringifiedHints.end(); it++)
			{
				s += VERTICAL_CHAR + it->at(i);
			}
			s += VERTICAL_CHAR + '\n';

			// If the last hasn't been reached yet, also render an interline row, like so:
   			// ║ ║ ║ ║ ║ ║
			if (i < maxHintSequenceLength - 1)
			{
				for (int i = 0; i < stringifiedHints.size(); i++)
				{
					s += VERTICAL_CHAR + pad(charWidth, " ");
				}
				s += VERTICAL_CHAR + '\n';
			}
		}

		return s;
	}
}