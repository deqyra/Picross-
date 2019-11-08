#include "picross_shell_state.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    PicrossShellState::PicrossShellState()
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
}