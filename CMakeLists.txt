cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Picross VERSION 0.1
                DESCRIPTION "A Picross grid solver"
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(tinyxml2 SHARED lib/tinyxml2/tinyxml2.cpp lib/tinyxml2/tinyxml2.hpp)

add_executable(picross  main.cpp
                        cli/cli_input.hpp                       cli/cli_input.cpp
                        cli/cli_command.hpp                     cli/cli_command.cpp
                        cli/cli_state.cpp                       cli/cli_state.hpp
                        cli/cli_menu.cpp                        cli/cli_menu.hpp
                        cli/commands/create_grid_command.cpp    cli/commands/create_grid_command.hpp
                        cli/commands/load_grid_command.cpp      cli/commands/load_grid_command.hpp
                        cli/commands/save_grid_command.cpp      cli/commands/save_grid_command.hpp
                        cli/commands/modify_grid_command.cpp    cli/commands/modify_grid_command.hpp
                        cli/commands/solve_grid_command.cpp     cli/commands/solve_grid_command.hpp
                        cli/commands/command_sequence.cpp       cli/commands/command_sequence.hpp
                        cli/commands/menu_command.cpp           cli/commands/menu_command.hpp
                        core/cell_t.cpp                         core/cell_t.hpp
                        core/grid.cpp                           core/grid.hpp
                        core/utility.cpp                        core/utility.hpp
                        io/invalid_xml_grid_error.cpp           io/invalid_xml_grid_error.hpp
                        io/text_grid_formatter.cpp              io/text_grid_formatter.hpp
                        io/tinyxml2_error.cpp                   io/tinyxml2_error.hpp
                        io/xml_grid_serializer.cpp              io/xml_grid_serializer.hpp
                        solving/solver.cpp                      solving/solver.hpp
                        solving/utility.cpp                     solving/utility.hpp
)
target_link_libraries(picross PUBLIC tinyxml2)