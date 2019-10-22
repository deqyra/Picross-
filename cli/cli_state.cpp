#include "cli_state.hpp"

#include <iostream>

#include "../core/grid.hpp"

namespace Picross
{
    CLIState::CLIState(std::istream& in, std::ostream& out, std::ostream& err) :
        _in(in.rdbuf()),
        _out(out.rdbuf()),
        _err(err.rdbuf()),
        _grid(0,0)
    {

    }

    std::istream& CLIState::in()
    {
        return _in;
    }

    std::ostream& CLIState::out()
    {
        return _out;
    }

    std::ostream& CLIState::err()
    {
        return _err;
    }

    Grid& CLIState::grid()
    {
        return _grid;
    }
}