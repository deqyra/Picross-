#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "../core/grid.hpp"

namespace Picross
{
    class Solver
    {
        public:
            Solver();
            virtual void solve(Grid& grid) = 0;
    };
}

#endif//SOLVER_HPP