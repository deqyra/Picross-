#include "picross_shell_state.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    PicrossShellState::PicrossShellState() :
        _mainGrid(0, 0),
        _workingGrid(0, 0)
    {

    }

    Grid& PicrossShellState::mainGrid()
    {
        return _mainGrid;
    }

    Grid& PicrossShellState::workingGrid()
    {
        return _workingGrid;
    }
} // namespace Picross