#ifndef CLI__PICROSS_CLI_STATE_HPP
#define CLI__PICROSS_CLI_STATE_HPP

#include <iostream>

#include "../core/grid.hpp"

namespace Picross
{
    class PicrossCLIState
    {
        private:    // Attributes
            Grid _grid;

        public:     // Public methods
            PicrossCLIState();
            Grid& grid();
    };
}

#endif//CLI__PICROSS_CLI_STATE_HPP