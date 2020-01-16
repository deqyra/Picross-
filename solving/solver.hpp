#ifndef SOLVING__SOLVER_HPP
#define SOLVING__SOLVER_HPP

#include <string>

#include "../core/grid.hpp"

namespace Picross
{
    class Solver
    {
        public:
            virtual ~Solver();
            virtual std::string name() = 0;
            virtual void solve(Grid& grid) = 0;
    };
} // namespace Picross

#endif//SOLVING__SOLVER_HPP