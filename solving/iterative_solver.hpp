#ifndef SOLVING__ITERATIVE_SOLVER_HPP
#define SOLVING__ITERATIVE_SOLVER_HPP

#include <string>

#include "solver.hpp"
#include "../core/grid.hpp"

namespace Picross
{
    class IterativeSolver : public Solver
    {
        public:
            virtual ~IterativeSolver();
            virtual std::string name();
            virtual void solve(Grid& grid);

        private:
            std::vector<cell_t> solveArray(std::vector<cell_t> array, std::vector<int> hints);
    };
}

#endif//SOLVING__ITERATIVE_SOLVER_HPP