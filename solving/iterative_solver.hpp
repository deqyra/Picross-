#ifndef SOLVING__ITERATIVE_SOLVER_HPP
#define SOLVING__ITERATIVE_SOLVER_HPP

#include <string>
#include <vector>

#include "solver.hpp"
#include "../core/grid.hpp"
#include "../core/cell_t.hpp"

namespace Picross
{
    class IterativeSolver : public Solver
    {
        public:
            virtual ~IterativeSolver();
            virtual std::string name();
            virtual void solve(Grid& grid);

        private:
            std::vector<cell_t> trivialArraySolving(std::vector<cell_t> array, std::vector<int> hints);
    };
}

#endif//SOLVING__ITERATIVE_SOLVER_HPP