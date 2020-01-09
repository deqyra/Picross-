#ifndef TOOLS__ARRAY_2D_HPP
#define TOOLS__ARRAY_2D_HPP

#include <vector>
#include <stdexcept>

#include "exceptions/unmatched_array_size_error.hpp"
#include "exceptions/index_out_of_bounds_error.hpp"

template<typename T>
class Array2D
{
    private:    // Attributes
        unsigned int _width;
        unsigned int _height;
        std::vector<T> _content;

        const std::function<bool(const T&)> _defaultValidityCheck = [] (const T&) {
            return true;
        };

    public:     // Public methods
        Array2D(unsigned int height, unsigned int width);

        unsigned int getHeight() const;
        unsigned int getWidth() const;

    // These return COPIES only. Modifications to the grid content and hints to be made through the appropriate methods.
        std::vector<T> getRow(int row) const;
        std::vector<T> getCol(int col) const;

    // Cell modification methods.
        T getCell(int row, int col) const;
        void setCell(int row, int col, T val, std::function<bool(const T&)> validityCheck = _defaultValidityCheck);
        void setCellRange(int i0, int in, int j0, int jn, T val, std::function<bool(const T&)> validityCheck = _defaultValidityCheck);
        void setRow(int row, std::vector<T> array, std::function<bool(const T&)> validityCheck = _defaultValidityCheck);
        void setCol(int col, std::vector<T> array, std::function<bool(const T&)> validityCheck = _defaultValidityCheck);

    // Useful checks.
        bool isValidRow(int row, bool throwOnFail = false) const;
        bool isValidCol(int col, bool throwOnFail = false) const;
        bool isValidCell(int row, int col, bool throwOnFail = false) const;
};

template<typename T>
Array2D<T>::Array2D(unsigned int height, unsigned int width) :
    _height(height),
    _width(width),
    _content(width * height)
{

}

template<typename T>
int Array2D<T>::getHeight() const
{
	return _height;
}

template<typename T>
int Array2D<T>::getWidth() const
{
	return _width;
}

template<typename T>
std::vector<T> Array2D<T>::getRow(int row) const
{
    // This will throw if the check fails (last parameter).
    isValidRow(row, true);

    auto begin = _content.begin() + (_width * row);
    return std::vector<cell_t>(begin, begin + _width);
}

template<typename T>
std::vector<T> Array2D<T>::getCol(int col) const
{
    // This will throw if the check fails (last parameter).
    isValidCol(col, true);

    std::vector<cell_t> columnVector(_height, CELL_CLEARED);

    auto it = _content.begin() + col;
    for (int i = 0; i < _height; i++)
    {
        columnVector[i] = (*it);
        it += _width;
    }
    return columnVector;
}

template<typename T>
T Array2D<T>::getCell(int row, int col) const
{
    // This will throw if the check fails (last parameter).
    isValidCell(row, col, true);

    return _content[(row * _width) + col];
}

template<typename T>
void Array2D<T>::setCell(int row, int col, T val, std::function<bool(const T&)> validityCheck)
{
    // This will throw if the checks fail (last parameter).
    isValidCell(row, col, true);
    // Auto-throw on invalid cell value.
    if (!validityCheck(val))
    {
            std::string s = "setCell: invalid cell value was provided.";
            throw std::invalid_argument(s.c_str());
    }

    _content[(row * _width) + col] = val;
}

template<typename T>
void Array2D<T>::setCellRange(int i0, int in, int j0, int jn, T val, std::function<bool(const T&)> validityCheck)
{
    // Auto-throw on invalid cell value.
    if (!validityCheck(val))
    {
        std::string s = "setCellRange: invalid cell value was provided.";
        throw std::invalid_argument(s.c_str());
    }

    // Throw if given range is invalid.
    if (!isValidRow(i0) || !isValidRow(in) || !isValidCol(j0) || !isValidCol(jn))
    {
        std::string s = "Range (" + std::to_string(i0) + ":" + std::to_string(in) + " ; " + std::to_string(j0) + ":" + std::to_string(jn) + ") is out of bound for grid of dimensions (" + std::to_string(_height) + ", " + std::to_string(_width) + ").";
        throw IndexOutOfBoundsError(s.c_str());
    }

    // Swap range extents if wrongly ordered.
    if (i0 > in)
    {
        std::swap(i0, in);
    }
    if (j0 > jn)
    {
        std::swap(j0, jn);
    }

    // Set given value for all cells in range.
    for (int i = i0; i <= in; i++)
    {
        for (int j = j0; j <= jn; j++)
        {
            _content[(i * _width) + j] = val;
        }
    }
}

template<typename T>
void Array2D<T>::setRow(int row, std::vector<T> array, std::function<bool(const T&)> validityCheck)
{
    // Auto-throw on invalid row index.
    isValidRow(row, true);

    // Check length of provided array.
    if (array.size() != _width)
    {
        std::string s = "setRow: passed array has size " + std::to_string(array.size()) + " but grid has width " + std::to_string(_width) + ".";
        throw UnmatchedArraySizeError(s);
    }

    for (int i = 0; i  < array.size(); i++)
    {
        // Auto-throw on invalid cell value.
        if (!validityCheck(array[i]))
        {
            std::string s = "setRow: invalid element value was provided.";
            throw std::invalid_argument(s.c_str());
        }
    }

    // Copy all cell values into row.
    for (int i = 0; i < _width; i++)
    {
        setCell(row, i, array[i]);
    }
}

template<typename T>
void Array2D<T>::setCol(int col, std::vector<T> array, std::function<bool(const T&)> validityCheck)
{
    // Auto-throw on invalid col index.
    isValidCol(col, true);

    // Check length of provided array.
    if (array.size() != _height)
    {
        std::string s = "setCol: passed array has size " + std::to_string(array.size()) + " but grid has height " + std::to_string(_height) + ".";
        throw UnmatchedArraySizeError(s);
    }

    for (int i = 0; i  < array.size(); i++)
    {
        // Auto-throw on invalid cell value.
        if (!validityCheck(array[i]))
        {
            std::string s = "setCol: invalid element value was provided.";
            throw std::invalid_argument(s.c_str());
        }
    }

    // Copy all cell values into col.
    for (int i = 0; i < _height; i++)
    {
        setCell(i, col, array[i]);
    }
}

template<typename T>
bool Array2D<T>::isValidRow(int row, bool throwOnFail = false) const
{
    bool valid = row >= 0 && row < _height;
    if (throwOnFail && !valid)
    {
        std::string s = "Invalid row " + std::to_string(row) + " for grid with height " + std::to_string(_height) + ".";
        throw IndexOutOfBoundsError(s);
    }
    return valid;
}

template<typename T>
bool Array2D<T>::isValidCol(int col, bool throwOnFail = false) const
{
    bool valid = col >= 0 && col < _width;
    if (throwOnFail && !valid)
    {
        std::string s = "Invalid column " + std::to_string(col) + " for grid with width " + std::to_string(_width) + ".";
        throw IndexOutOfBoundsError(s);
    }
    return valid;
}

template<typename T>
bool Array2D<T>::isValidCell(int row, int col, bool throwOnFail = false) const
{
    bool valid = isValidRow(row) && isValidCol(col);
    if (throwOnFail && !valid)
    {
        std::string s = "Invalid cell (" + std::to_string(row) + ", " + std::to_string(col) + ") for grid with dimensions (" + std::to_string(_height) + ", " + std::to_string(_width) + ").";
        throw IndexOutOfBoundsError(s);
    }
    return valid;
}

#endif//TOOLS__ARRAY_2D_HPP