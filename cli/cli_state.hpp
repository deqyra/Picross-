#ifndef CLI_STATE_HPP
#define CLI_STATE_HPP

#include <iostream>

#include "../core/grid.hpp"

namespace Picross
{
    class CLIState
    {
        private:    // Attributes
            std::istream _in;
            std::ostream _out;
            std::ostream _err;

            Grid _grid;

        public:     // Public methods
            CLIState(std::istream& in, std::ostream& out, std::ostream& err);

            std::istream& in();
            std::ostream& out();
            std::ostream& err();

            Grid& grid();
    };
}

#endif//CLI_STATE_HPP