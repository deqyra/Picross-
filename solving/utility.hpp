#ifndef SOLVING__UTILITY_HPP
#define SOLVING__UTILITY_HPP

#include <vector>
#include <memory>

#include "solver.hpp"

namespace Picross
{
    inline static const int SOLVER_COUNT = 0;

    std::vector<std::shared_ptr<Solver>> instantiateAllSolvers();
}

#endif//SOLVING__UTILITY_HPP