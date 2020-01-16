#include "catch2_custom_string_makers.hpp"
#include "../lib/catch2/catch2.hpp"
#include "../core/grid.hpp"
#include "../io/text_grid_formatter.hpp"

// Doesn't work for some reason
namespace Catch
{
    std::string StringMaker<Picross::Grid>::convert(Picross::Grid const& grid)
    {
        Picross::TextGridFormatter txt = Picross::TextGridFormatter();
        return txt.renderGridWithHints(grid);
    }
} // namespace Catch
