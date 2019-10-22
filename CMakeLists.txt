# CMake configuration
    cmake_minimum_required(VERSION 3.1...3.15)

    if(${CMAKE_VERSION} VERSION_LESS 3.12)
        cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
    endif()

# Project general info
    set(PROJECT_MAINTAINER_NAME "deqyra")
    set(PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com")

# Project folder structure info
    set(BUILD_DIR "build")
    set(SOURCE_FROM_BUILD "..")

# Header configuration variables
    set(PROJECT_VERSION "0.1")
    set(PROJECT_NAME "PICROSS ENGINE")

    # Copyright notice generation
        set(PROJECT_START_YEAR "2019")
        string(TIMESTAMP CURRENT_YEAR "%Y")
        # "(C) 2019"
        string(CONCAT PROJECT_COPYRIGHT "(C) " ${PROJECT_START_YEAR})
        if(NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR)
            # "(C) 2019-2020"
            string(CONCAT PROJECT_COPYRIGHT ${PROJECT_COPYRIGHT} "-" ${CURRENT_YEAR})
        endif()
        # "(C) 2019[-2020] deqyra <deqyra@gmail.com>"
        string(CONCAT PROJECT_COPYRIGHT ${PROJECT_COPYRIGHT} " " ${PROJECT_MAINTAINER_NAME} " <" ${PROJECT_MAINTAINER_EMAIL} ">")

# Header configuration
configure_file("cmake_defines.config.hpp" "${SOURCE_FROM_BUILD}/cmake_defines.hpp")

# Project definition
    project(PicrossEngine   VERSION ${PROJECT_VERSION}
                            DESCRIPTION "A Picross grid solver"
                            LANGUAGES CXX
    )

# C++ standard version
    set(CMAKE_CXX_STANDARD 17)

# Build tinyxml2
    add_library(tinyxml2 SHARED lib/tinyxml2/tinyxml2.cpp lib/tinyxml2/tinyxml2.hpp)

# Build project
    add_executable(picross  main.cpp
                            cmake_defines.config.hpp                cmake_defines.hpp
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