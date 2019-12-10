#ifndef TESTS__CATCH2_CUSTOM_STRING_FORMATTERS
#define TESTS__CATCH2_CUSTOM_STRING_FORMATTERS

#include "../lib/catch2/catch2.hpp"
#include "../core/grid.hpp"

namespace Catch
{
    template<>
    struct StringMaker<Picross::Grid>
    {
        static std::string convert(Picross::Grid const& grid);
    };
}

#endif//TESTS__CATCH2_CUSTOM_STRING_FORMATTERS