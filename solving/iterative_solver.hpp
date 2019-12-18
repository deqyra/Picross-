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
            void gridIteration(Grid& grid);
            std::vector<cell_t> arrayIteration(std::vector<cell_t> array, const std::vector<int>& hints);

            // Check every possible cell given an a array and its associated hints. Cross appropriate cells when possible.
            static std::vector<cell_t> arrayTrivialChecking(std::vector<cell_t> array, std::vector<int> hints);
            // Cross cells that are out of bounds for any checked block according to given hints.
            static std::vector<cell_t> arrayTrivialCrossing(std::vector<cell_t> array, std::vector<int> hints);
            // Cross cells that are in spaces where no checked blocks fit according to given hints.
            static std::vector<cell_t> arrayForceCrossing(std::vector<cell_t> array, std::vector<int> hints);
            // Check additional cells in blocks that are close enough to a border or a crossed cell according to given hints.
            static std::vector<cell_t> arrayGluing(std::vector<cell_t> array, std::vector<int> hints);
            // Join separate checked blocks where appropriate according to given hints, by checking cells in between them.
            static std::vector<cell_t> arrayJoin(std::vector<cell_t> array, std::vector<int> hints);
            // Split separate checked blocks where appropriate according to given hints, by crossing cells in between them.
            static std::vector<cell_t> arraySplit(std::vector<cell_t> array, std::vector<int> hints);
    };
}

#endif//SOLVING__ITERATIVE_SOLVER_HPP