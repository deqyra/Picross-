#ifndef PICROSS_SHELL__PICROSS_SHELL_STATE_HPP
#define PICROSS_SHELL__PICROSS_SHELL_STATE_HPP

#include "../core/grid.hpp"

namespace Picross
{
    class PicrossShellState
    {
        private:    // Attributes
            Grid _mainGrid;
            Grid _workingGrid;

        public:
            PicrossShellState();
            
            Grid& mainGrid();
            Grid& workingGrid();
    };
}

#endif//PICROSS_SHELL__PICROSS_SHELL_STATE_HPP