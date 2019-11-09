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
    set( TOOLS_LIB_NAME "Tools" )
    set( CORE_LIB_NAME "${PROJECT_TARGET_NAME}.Core" )
    set( SOLVER_LIB_NAME "${PROJECT_TARGET_NAME}.Solvers" )
    set( IO_LIB_NAME "${PROJECT_TARGET_NAME}.IO" )
    set( CLI_LIB_NAME "${PROJECT_TARGET_NAME}.CLI" )
    set( SHELL_LIB_NAME "${PROJECT_TARGET_NAME}.Shell" )
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
    # C++17 requires at least gcc 7 to be compiled
    set( CMAKE_CXX_STANDARD 17 )

# Build tinyxml2
    set( TINYXML2_NAME "tinyxml2" )
    add_library( ${TINYXML2_NAME} SHARED
                lib/tinyxml2/tinyxml2.cpp   lib/tinyxml2/tinyxml2.hpp)

# Build project
    # Build string util lib
        add_library( ${TOOLS_LIB_NAME} SHARED
                    tools/string_tools.cpp                              tools/string_tools.hpp
                                                                        tools/iterable_tools.hpp
                                                                        tools/micro_shell/micro_shell.hpp
                                                                        tools/micro_shell/micro_shell_codes.hpp
                                                                        tools/micro_shell/micro_shell_command.hpp
                                                                        tools/make_basic_exception.hpp
                    tools/exceptions/index_out_of_bounds_error.cpp      tools/exceptions/index_out_of_bounds_error.hpp
                    tools/exceptions/file_not_found_error.cpp           tools/exceptions/file_not_found_error.hpp 
                    tools/cli/cli_input.hpp                             tools/cli/cli_input.cpp
                    tools/cli/cli_streams.cpp                           tools/cli/cli_streams.hpp
                                                                        tools/cli/cli_command.hpp
                                                                        tools/cli/cli_menu.hpp
                                                                        tools/cli/menu_command.hpp
                                                                        tools/cli/command_sequence.hpp )

    # Build core lib
        add_library( ${CORE_LIB_NAME} SHARED
                    core/cell_t.cpp                                             core/cell_t.hpp
                    core/grid.cpp                                               core/grid.hpp
                    core/utility.cpp                                            core/utility.hpp
                    core/exceptions/invalid_cell_value_error.cpp                core/exceptions/invalid_cell_value_error.hpp
                    core/exceptions/invalid_grid_hints_error.cpp                core/exceptions/invalid_grid_hints_error.hpp
                    core/exceptions/unrecognized_cell_value_error.cpp           core/exceptions/unrecognized_cell_value_error.hpp )
                    target_link_libraries( ${CORE_LIB_NAME} PUBLIC ${TOOLS_LIB_NAME} )

    # Build solver lib
        add_library( ${SOLVER_LIB_NAME} SHARED
                    solving/solver.cpp                      solving/solver.hpp
                    solving/utility.cpp                     solving/utility.hpp)
        target_link_libraries( ${SOLVER_LIB_NAME} PUBLIC ${CORE_LIB_NAME} )

    # Build IO lib
        add_library( ${IO_LIB_NAME} SHARED
                    io/xml_grid_serializer.cpp                  io/xml_grid_serializer.hpp
                    io/text_grid_formatter.cpp                  io/text_grid_formatter.hpp
                    io/exceptions/invalid_xml_grid_error.cpp    io/exceptions/invalid_xml_grid_error.hpp
                    io/exceptions/tinyxml2_error.cpp            io/exceptions/tinyxml2_error.hpp )
        target_link_libraries( ${IO_LIB_NAME} PUBLIC ${CORE_LIB_NAME} ${TINYXML2_NAME} )

    # Build CLI lib
        add_library( ${CLI_LIB_NAME} SHARED
                    picross_cli/picross_cli_state.cpp       picross_cli/picross_cli_state.hpp
                    picross_cli/create_grid_command.cpp     picross_cli/create_grid_command.hpp
                    picross_cli/load_grid_command.cpp       picross_cli/load_grid_command.hpp
                    picross_cli/save_grid_command.cpp       picross_cli/save_grid_command.hpp
                    picross_cli/modify_grid_command.cpp     picross_cli/modify_grid_command.hpp
                    picross_cli/solve_grid_command.cpp      picross_cli/solve_grid_command.hpp )
        target_link_libraries( ${CLI_LIB_NAME} PUBLIC
                                ${TOOLS_LIB_NAME}
                                ${CORE_LIB_NAME}
                                ${SOLVER_LIB_NAME}
                                ${IO_LIB_NAME}
                                ${SHELL_LIB_NAME} )
    
    # Build shell lib
        add_library( ${SHELL_LIB_NAME} SHARED
                    picross_shell/picross_shell_commit_command.cpp      picross_shell/picross_shell_commit_command.hpp
                    picross_shell/picross_shell_display_command.cpp     picross_shell/picross_shell_display_command.hpp
                    picross_shell/picross_shell_exit_command.cpp        picross_shell/picross_shell_exit_command.hpp
                    picross_shell/picross_shell_hints_command.cpp       picross_shell/picross_shell_hints_command.hpp
                    picross_shell/picross_shell_rollback_command.cpp    picross_shell/picross_shell_rollback_command.hpp
                    picross_shell/picross_shell_check_command.cpp       picross_shell/picross_shell_check_command.hpp
                    picross_shell/picross_shell_cross_command.cpp       picross_shell/picross_shell_cross_command.hpp
                    picross_shell/picross_shell_clear_command.cpp       picross_shell/picross_shell_clear_command.hpp
                    picross_shell/picross_shell_state.cpp               picross_shell/picross_shell_state.hpp )
        target_link_libraries( ${SHELL_LIB_NAME} PUBLIC ${TOOLS_LIB_NAME} )

    # Build executable
        add_executable( ${EXECUTABLE_NAME}
                        main.cpp
                        cmake_defines.config.hpp     cmake_defines.hpp)
        target_link_libraries( ${EXECUTABLE_NAME} PUBLIC ${CLI_LIB_NAME} )
        add_dependencies( ${EXECUTABLE_NAME} ${COPY_RESOURCES_TARGET_NAME} )

# Build and run tests
    enable_testing( )
    add_executable( ${TEST_TARGET_NAME}  tests/main.cpp                         tests/custom_generators.hpp
                                        tests/core/test_cell_t.cpp              tests/template_scenario.hpp
                                        tests/core/test_utility.cpp
                                        tests/core/test_grid.cpp
                                        tests/cli/test_picross_cli_state.cpp
                                        tests/tools/test_cli_input.cpp
                                        tests/tools/test_cli_streams.cpp
                                        tests/tools/test_string_tools.cpp
                                        tests/io/generate_static_grids.cpp      tests/io/generate_static_grids.hpp
                                        tests/io/test_xml_grid_serializer.cpp
                                        tests/io/test_text_grid_formatter.cpp )
    target_link_libraries( ${TEST_TARGET_NAME} PUBLIC ${CORE_LIB_NAME} ${CLI_LIB_NAME} ${IO_LIB_NAME} ${TOOLS_LIB_NAME} )
    add_dependencies( ${TEST_TARGET_NAME} ${COPY_RESOURCES_TARGET_NAME} )

    add_test( NAME "tests" COMMAND ${TEST_TARGET_NAME} )
    add_custom_command( TARGET ${TEST_TARGET_NAME}
                        COMMENT "Running tests"
                        POST_BUILD
                        COMMAND "./tests" )
