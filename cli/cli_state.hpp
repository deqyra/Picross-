#ifndef CLI_STATE_HPP
#define CLI_STATE_HPP

#include <iostream>

#include "../core/grid.hpp"

namespace Picross
{
    class CLIState
    {
        private:
            std::istream _in;
            std::ostream _out;
            std::ostream _err;

            Grid _grid;

        public:
            CLIState(std::istream& in, std::ostream& out, std::ostream& err);
            ~CLIState();

            std::istream& in();
            std::ostream& out();
            std::ostream& err();

            Grid& grid();
    };
}

#endif//CLI_STATE_HPP