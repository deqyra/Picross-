#include "picross_cli_state.hpp"

#include <iostream>

#include "../core/grid.hpp"

namespace Picross
{
    PicrossCLIState::PicrossCLIState() :
        _grid(0,0)
    {

    }

    Grid& PicrossCLIState::grid()
    {
        return _grid;
    }
}