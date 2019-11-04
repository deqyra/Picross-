#ifndef TESTS__TEMPLATE_SCENARIO_HPP
#define TESTS__TEMPLATE_SCENARIO_HPP

#include "../lib/catch2/catch2.hpp"
#define TEMPLATE_SCENARIO( ... ) TEMPLATE_TEST_CASE( "Scenario: " __VA_ARGS__ )

#endif//TESTS__TEMPLATE_SCENARIO_HPP