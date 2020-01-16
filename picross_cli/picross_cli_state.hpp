#ifndef PICROSS_CLI__CLI_PICROSS_CLI_STATE_HPP
#define PICROSS_CLI__CLI_PICROSS_CLI_STATE_HPP

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
} // namespace Picross

#endif//PICROSS_CLI__CLI_PICROSS_CLI_STATE_HPP