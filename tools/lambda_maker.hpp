#ifndef TOOLS__LAMBDA_MAKER_HPP
#define TOOLS__LAMBDA_MAKER_HPP

#include <functional>

namespace LambdaMaker
{
    template<typename T>
    std::function<bool(const T&)> greater_than(const T& value);

    template<typename T>
    std::function<bool(const T&)> greater_equal(const T& value);

    template<typename T>
    std::function<bool(const T&)> less_than(const T& value);

    template<typename T>
    std::function<bool(const T&)> less_equal(const T& value);

    template<typename T>
    std::function<bool(const T&)> between(const T& low, const T& high);

    template<typename T>
    std::function<bool(const T&)> strictly_between(const T& low, const T& high);
}

#endif//TOOLS__LAMBDA_MAKER_HPP