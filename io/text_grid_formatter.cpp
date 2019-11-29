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
		//
		// Several numbers are needed before doing all that:
		// - the character width of the maximum column hint, which will affect the cell width of the whole grid,
		// - the width of the widest stringified row hint vector, which will affect the width of the padding block.

		int width = grid.getWidth();
		int height = grid.getHeight();

		// Find the character width of the maximum column hint.
		int charWidth = findCharWidthFromColHintSequences(grid.getAllColHints());
		// The above may return garbage in case of no column hints, set it back to 1.
		if (charWidth < 1) charWidth = 1;

		// Stringify the row hint vectors.
		std::vector<std::string> stringifiedRowHints = stringifyRowHintSequences(grid.getAllRowHints());
		// Generated row hint strings are all the same length.
		int rowHintStringLength = stringifiedRowHints[0].length();

		// Stringify the col hint vectors.
		std::vector<std::vector<std::string>> stringifiedColHints = stringifyColHintSequences(grid.getAllColHints(), charWidth);

		// Get the height of the col hints in the final render (size of any vector in the vector of stringified hint vectors).
		int colHintsStringHeight = stringifiedColHints[0].size();

		// Separately render all four aforementioned blocks.
		std::string paddingBlock = padBlock(rowHintStringLength, colHintsStringHeight, " ");
		std::string hHintsStr = renderStringifiedRowHints(stringifiedRowHints);
		std::string vHintsStr = renderStringifiedColHintSequenceVector(stringifiedColHints);
		std::string gridStr = renderGrid(grid, emptyCrossedCells, charWidth);

		// In all blocks (if appropriate), remove the final \n to negate useless newline concatenation.
		if (paddingBlock.length()) paddingBlock.pop_back();
		if (vHintsStr.length()) vHintsStr.pop_back();
		if (hHintsStr.length()) hHintsStr.pop_back();
		gridStr.pop_back();

		// Combine all blocks to get the final render.
		std::string s;
		if (colHintsStringHeight)
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

	int TextGridFormatter::findCharWidthFromColHintSequences(const std::vector<std::vector<int>>& hintVectors)
	{
		// Lambda which takes in a vector of ints (assumed unsigned) and returns the max int from it.
		auto lambda_maxElement = [] (const std::vector<int>& vec) -> int
		{
			// Find the max element.
			auto it = std::max_element(vec.begin(), vec.end());
			// Return it (or 0 if the vector was empty).
			return it != vec.end() ? (*it) : 0;
		};

		// Vector which will receive the greatest hint from each column hint sequence.
		std::vector<int> maxColHints = std::vector<int>();
		maxColHints.resize(hintVectors.size());

		// Retrieve the greatest hint in each column of the grid.
		std::transform(hintVectors.begin(), hintVectors.end(), maxColHints.begin(), lambda_maxElement);

		// Now find the max hint from all the max col hints.
		int maxHint = *(std::max_element(maxColHints.begin(), maxColHints.end()));

		// Given the biggest column hint in all the grid, find how much char width is needed to display it.
		return floor(log10(maxHint)) + 1;
	}

	std::string TextGridFormatter::renderRow(const std::vector<cell_t>& row, bool emptyCrossedCells, int cellWidth)
	{
		// A row looks like this (cellWidth == 1):
		// ║■║ ║■║ ║ ║ ... ║×║×║×║ ║
		//
		// (cellWidth == 2):
		// ║■■║  ║■■║  ║  ║ ... ║××║××║××║  ║

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

	std::string TextGridFormatter::renderTopLine(int width, int cellWidth)
	{
		// The top line looks like this (cellWidth == 1):
		// ╔═╦═╦═╦═╦═ ... ╦═╦═╦═╦═╦═╗
		//
		// (cellWidth == 2):
		// ╔══╦══╦══╦══╦══ ... ╦══╦══╦══╦══╦══╗

		std::string s;
		s += TOP_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += pad(cellWidth, HORIZONTAL_CHAR) + TOP_CROSS_CHAR;
		}
		s += pad(cellWidth, HORIZONTAL_CHAR) + TOP_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderInterline(int width, int cellWidth)
	{
		// An interline looks like this (cellWidth == 1):
		// ╠═╬═╬═╬═╬═╬═ ... ╬═╬═╬═╬═╣
		//
		// (cellWidth == 2):
		// ╠══╬══╬══╬══╬══╬══ ... ╬══╬══╬══╬══╣

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
		// The bottom line looks like this (cellWidth == 1):
		// ╚═╩═╩═╩═╩═ ... ╩═╩═╩═╩═╩═╝
		//
		// (cellWidth == 2)
		// ╚══╩══╩══╩══╩══ ... ╩══╩══╩══╩══╩══╝
		
		std::string s;
		s += BOTTOM_LEFT_CHAR;

		for (int i = 0; i < width - 1; i++)
		{
			s += pad(cellWidth, HORIZONTAL_CHAR) + BOTTOM_CROSS_CHAR;
		}
		s += pad(cellWidth, HORIZONTAL_CHAR) + BOTTOM_RIGHT_CHAR + '\n';

		return s;
	}

	std::string TextGridFormatter::renderStringifiedRowHints(const std::vector<std::string>& stringifiedHintVectors)
	{
		// Guarantee: input strings are all the same length.
		int stringLength = stringifiedHintVectors[0].length();

		std::string s;
		for (auto it = stringifiedHintVectors.begin(); it != stringifiedHintVectors.end(); it++)
		{
			// Generate a separator string like:
			// ══════════ ... ═════════
			s += pad(stringLength, HORIZONTAL_CHAR) + '\n';

			// Concatenate the padded hints.
			s += *it + '\n';
		}
		// Generate bottom separator string.
		s += pad(stringLength, HORIZONTAL_CHAR) + '\n';

		return s;
	}

	std::string TextGridFormatter::renderStringifiedColHintSequenceVector(const std::vector<std::vector<std::string>>& stringifiedHintVectors)
	{
		// Guarantee: input vectors are all the same size.
		int sequenceLength = stringifiedHintVectors[0].size();

		std::string s;
		// For every row of hints across the hint vectors...
		for (int i = 0; i < sequenceLength; i++)
		{
			// Render the row across the vectors, which will ultimately look like that:
			// ║2║ ║ ║ ║1║

			for (auto it = stringifiedHintVectors.begin(); it != stringifiedHintVectors.end(); it++)
			{
				s += VERTICAL_CHAR + it->at(i);
			}
			s += VERTICAL_CHAR + '\n';
		}

		return s;
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

	std::vector<std::string> TextGridFormatter::stringifyRowHintSequences(const std::vector<std::vector<int>>& hintVectors)
	{
		// Vector which will receive the stringified hint sequences.
		std::vector<std::string> stringifiedHints;
		int maxStringLength = 0;

		// For each hint sequence...
		for (auto it = hintVectors.begin(); it != hintVectors.end(); it++)
		{
			// Turn it into a string.
			std::string stringifiedVector = StringTools::iterableToString(*it);

			// Update the max generated string length if needed.
			if (stringifiedVector.length() > maxStringLength)
			{
				maxStringLength = stringifiedVector.length();
			}
			stringifiedHints.push_back(stringifiedVector);
		}

		// Pad all generated strings when needed.
		for (auto it = stringifiedHints.begin(); it != stringifiedHints.end(); it++)
		{
			int padLength = maxStringLength - it->length();
			*it = pad(padLength, " ") + *it;
		}

		// Example:
		// 		hintVectors:
		// 		{
		//			{3, 4, 1},
		// 			{2, 2, 2, 2},
		// 			{7, 2}
		// 		}
		//
		//		will give
		//
		// 		stringifiedHints:
		// 		{
		//			"  3 4 1",
		//			"2 2 2 2",
		//			"    7 2"
		// 		}
		// 		and maxStringLength: 7

		return stringifiedHints;
	}

	std::vector<std::vector<std::string>> TextGridFormatter::stringifyColHintSequences(const std::vector<std::vector<int>>& hintVectors, int charWidth)
	{
		// Get the size of the biggest hint sequence.
		int maxHintSequenceLength = IterTools::maxIterableLength(hintVectors);

		// Vector which will receive the sequences of stringified hints.
		std::vector<std::vector<std::string>> stringifiedHints = std::vector<std::vector<std::string>>();

		// Transform int hint vectors into padded string hint vectors.
		for (auto it = hintVectors.begin(); it != hintVectors.end(); it++)
		{
			// Turn each int hint sequence into a string hint sequence, each of which is space-padded to match charWidth.
			// Every output string sequence will be the same size as all others, as empty strings will be prepended to each in order to match maxHintSequenceLength.
			stringifiedHints.push_back(stringifyColHintsInVector(*it, charWidth, maxHintSequenceLength));
		}

		// Example:
		// 		hintVectors:
		// 		{
		//			{3, 4, 1},
		// 			{2, 2, 2, 2},
		// 			{7, 2}
		// 		}
		//
		//		will give
		//
		// 		stringifiedHints:
		// 		{
		//			{" ", "3", "4", "1"},
		//			{"2", "2", "2", "2"},
		//			{" ", " ", "7", "2"}
		// 		}

		return stringifiedHints;
	}

	std::vector<std::string> TextGridFormatter::stringifyColHintsInVector(const std::vector<int>& hints, int charWidth, int maxHintSequenceLength)
	{
		// Vector which will receive stringified hints.
		std::vector<std::string> stringifiedHints;

		// Calculate how many empty strings need to be generated and prepended to the final vector.
		// This is so the hints, once rendered, will all be the same size vertically, like so: (double-quotes added for clarity)
		// ║" 2"║"  "║"  "║"  "║"10"║
		// ║" 1"║"10"║" 4"║"  "║" 5"║
		int hintSequenceLengthDiff = maxHintSequenceLength - hints.size();

		// Generate empty strings and prepend them to the final vector.
		for (int i = 0; i < hintSequenceLengthDiff; i++)
		{
			stringifiedHints.push_back(pad(charWidth, " "));
		}

		// Transform int hints into space-padded string hints and append them to the final vector.
		for (auto it = hints.begin(); it != hints.end(); it++)
		{
			// For each hint in the sequence, strigify and pad it.
			stringifiedHints.push_back(stringifyAndPadHint(*it, charWidth));
		}

		return stringifiedHints;
	}

	std::string TextGridFormatter::stringifyAndPadHint(const int& hint, int charWidth)
	{
		// Stringify the hint.
		std::string s = std::to_string(hint);
		// Calculate needed padding width.
		int padLength = charWidth - s.length();

		// Prepend padding if needed.
		if (padLength > 0)
		{
			s = pad(padLength, " ") + s;
		}

		return s;
	}
}