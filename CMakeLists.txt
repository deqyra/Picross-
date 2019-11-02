# CMake configuration
    cmake_minimum_required(VERSION 3.1...3.15)

    if( ${CMAKE_VERSION} VERSION_LESS 3.12)
        cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} )
    endif( )

# Project general info
    set( PROJECT_MAINTAINER_NAME "François Brachais" )
    set( PROJECT_MAINTAINER_EMAIL "deqyra@gmail.com" )

# Project target structure
    set( PROJECT_TARGET_NAME "PicrossEngine" )
    set( STRING_UTIL_LIB_NAME "StringUtil" )
    set( CORE_LIB_NAME "${PROJECT_TARGET_NAME}.Core" )
    set( SOLVER_LIB_NAME "${PROJECT_TARGET_NAME}.Solvers" )
    set( IO_LIB_NAME "${PROJECT_TARGET_NAME}.IO" )
    set( CLI_LIB_NAME "${PROJECT_TARGET_NAME}.CLI" )
    set( EXECUTABLE_NAME ${PROJECT_TARGET_NAME} )
    set( TEST_TARGET_NAME "tests" )
    set( COPY_RESOURCES_TARGET_NAME "copy_resources" )

# Copy resource folder
add_custom_target( ${COPY_RESOURCES_TARGET_NAME} )
add_custom_command( TARGET ${COPY_RESOURCES_TARGET_NAME}
                    COMMENT "Copying resource folder"
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ./resources )

# Header configuration variables
    set( PROJECT_VERSION "0.1" )
    set( PROJECT_NAME "PICROSS ENGINE" )

    # Copyright notice generation
        set( PROJECT_START_YEAR "2019" )
        string(TIMESTAMP CURRENT_YEAR "%Y" )
        # "(C) 2019"
        set( PROJECT_COPYRIGHT "(C) ${PROJECT_START_YEAR}" )
        if( NOT CURRENT_YEAR EQUAL PROJECT_START_YEAR )
            # "(C) 2019-2020"
            set( PROJECT_COPYRIGHT "${PROJECT_COPYRIGHT}-${CURRENT_YEAR}" )
        endif( )
        # "(C) 2019[-2020] deqyra <deqyra@gmail.com>"
        set( PROJECT_COPYRIGHT "${PROJECT_COPYRIGHT} ${PROJECT_MAINTAINER_NAME} <${PROJECT_MAINTAINER_EMAIL}>" )

# Header configuration
    configure_file( "cmake_defines.config.hpp" "${CMAKE_SOURCE_DIR}/cmake_defines.hpp" )

# Project definition
    project( ${PROJECT_TARGET_NAME} VERSION ${PROJECT_VERSION}
                                    DESCRIPTION "A Picross grid solver"
                                    LANGUAGES CXX)

# C++ standard version
    set( CMAKE_CXX_STANDARD 17 )

# Build tinyxml2
    set( TINYXML2_NAME "tinyxml2" )
    add_library( ${TINYXML2_NAME} SHARED
                lib/tinyxml2/tinyxml2.cpp   lib/tinyxml2/tinyxml2.hpp)

# Build project
    # Build string util lib
        add_library( ${STRING_UTIL_LIB_NAME} SHARED
                    string/utility.cpp      string/utility.hpp )

    # Build core lib
        add_library( ${CORE_LIB_NAME} SHARED
                    core/cell_t.cpp     core/cell_t.hpp
                    core/grid.cpp       core/grid.hpp
                    core/utility.cpp    core/utility.hpp)
                    target_link_libraries( ${CORE_LIB_NAME} PUBLIC ${STRING_UTIL_LIB_NAME} )

    # Build solver lib
        add_library( ${SOLVER_LIB_NAME} SHARED
                    solving/solver.cpp                      solving/solver.hpp
                    solving/utility.cpp                     solving/utility.hpp)
        target_link_libraries( ${SOLVER_LIB_NAME} PUBLIC ${CORE_LIB_NAME} )

    # Build IO lib
        add_library( ${IO_LIB_NAME} SHARED
                    io/invalid_xml_grid_error.cpp   io/invalid_xml_grid_error.hpp
                    io/text_grid_formatter.cpp      io/text_grid_formatter.hpp
                    io/tinyxml2_error.cpp           io/tinyxml2_error.hpp
                    io/xml_grid_serializer.cpp      io/xml_grid_serializer.hpp )
        target_link_libraries( ${IO_LIB_NAME} PUBLIC ${CORE_LIB_NAME} ${TINYXML2_NAME} )

    # Build CLI lib
        add_library( ${CLI_LIB_NAME} SHARED
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
                    cli/commands/menu_command.cpp           cli/commands/menu_command.hpp )
        target_link_libraries( ${CLI_LIB_NAME} PUBLIC
                            ${STRING_UTIL_LIB_NAME}
                            ${CORE_LIB_NAME}
                            ${SOLVER_LIB_NAME}
                            ${IO_LIB_NAME} )
    
    # Build executable
        add_executable( ${EXECUTABLE_NAME}
                        main.cpp
                        cmake_defines.config.hpp     cmake_defines.hpp)
        target_link_libraries( ${EXECUTABLE_NAME} PUBLIC ${CLI_LIB_NAME} )
        add_dependencies( ${EXECUTABLE_NAME} ${COPY_RESOURCES_TARGET_NAME} )

# Build and run tests
    enable_testing( )
    add_executable( ${TEST_TARGET_NAME}  tests/main.cpp
                                        tests/custom_generators.hpp
                                        tests/core/test_cell_t.cpp
                                        tests/core/test_utility.cpp
                                        tests/core/test_grid.cpp
                                        tests/cli/test_cli_state.cpp
                                        tests/cli/test_cli_input.cpp
                                        tests/string/test_utility.cpp
                                        tests/io/test_xml_grid_serializer.cpp )
    target_link_libraries( ${TEST_TARGET_NAME} PUBLIC ${CORE_LIB_NAME} ${CLI_LIB_NAME} )
    add_dependencies( ${TEST_TARGET_NAME} ${COPY_RESOURCES_TARGET_NAME} )

    add_test( NAME "tests" COMMAND ${TEST_TARGET_NAME} )
    add_custom_command( TARGET ${TEST_TARGET_NAME}
                        COMMENT "Running tests"
                        POST_BUILD
                        COMMAND "./tests" )
