#ifndef SOLVING_UTILITY_HPP
#define SOLVING_UTILITY_HPP

#include <vector>
#include <memory>

#include "solver.hpp"

namespace Picross
{
    inline static const int SOLVER_COUNT = 0;

    std::vector<std::shared_ptr<Solver>> instantiateAllSolvers();
}

#endif//SOLVING_UTILITY_HPP