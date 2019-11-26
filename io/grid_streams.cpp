#include "grid_streams.hpp"

#include <ostream>
#include "../core/grid.hpp"
#include "text_grid_formatter.hpp"

std::ostream& operator<<(std::ostream& os, Picross::Grid const& grid)
{
    // Print the input grid with hints.
    Picross::TextGridFormatter txt = Picross::TextGridFormatter();
    os << txt.renderGridWithHints(grid);
    return os;
}
