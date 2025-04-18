# -----------------------------------------------------------------------------
# Function to add static library
# -----------------------------------------------------------------------------
function(add_static_library NAME VERSION)
    # Add library target
    add_library(${NAME} STATIC)

    # Get SOVERSION
    string(
        REGEX MATCH
              "^([0-9]+)"
              SOVERSION
              ${VERSION}
    )

    # Set target version
    set_target_properties(
        ${NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SOVERSION}
    )

    #Code Coverage
    if(CODE_COVERAGE)
        add_code_coverage(${NAME})
    endif()

    # Install
    install(
        TARGETS ${NAME}
        ARCHIVE
        LIBRARY
        RUNTIME
    )

    # Install headers
    install(DIRECTORY headers/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".*hpp")
endfunction()

# -----------------------------------------------------------------------------
# Function to add shared library
# -----------------------------------------------------------------------------
function(add_shared_library NAME VERSION)
    # Add library target
    add_library(${NAME} SHARED)

    # Get SOVERSION
    string(
        REGEX MATCH
              "^([0-9]+)"
              SOVERSION
              ${VERSION}
    )

    # Set target version
    set_target_properties(
        ${NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SOVERSION}
    )

    #Code Coverage
    if(BUILD_WITH_CODE_COVERAGE)
        # add_code_coverage(${NAME})
    endif()

    # Install binaries
    install(
        TARGETS ${NAME}
        ARCHIVE
        LIBRARY
        RUNTIME
    )

    # Install headers
    install(DIRECTORY headers/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".*hpp")
endfunction()



# -----------------------------------------------------------------------------
# Function to add Interface library
# -----------------------------------------------------------------------------
function(add_interface_library NAME VERSION)
    # Add library target
    add_library(${NAME} INTERFACE)

    # Get SOVERSION
    string(
        REGEX MATCH
              "^([0-9]+)"
              SOVERSION
              ${VERSION}
    )

    # Set target version
    set_target_properties(
        ${NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SOVERSION}
    )

    #Code Coverage
    if(BUILD_WITH_CODE_COVERAGE)
        # add_code_coverage(${NAME})
    endif()

    # Install Interfaces
    install(
        TARGETS ${NAME}
        ARCHIVE
        LIBRARY
        RUNTIME
    )

    # Install headers
    install(DIRECTORY headers/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN ".*hpp")
endfunction()

# -----------------------------------------------------------------------------
# Function to make Doxygen Documentation
# -----------------------------------------------------------------------------
function(make_doxygen_doc NAME VERSION DESCRIPTION)
    # Import Doxygen if it is not already the case
    find_package(Doxygen REQUIRED dot)
    if(DOXYGEN_FOUND)
        message("🟢 Add documentation for ${NAME}")

        # set input and output files
        set(DOXYGEN_PROJECT_NAME ${NAME})
        set(DOXYGEN_PROJECT_VERSION ${VERSION})
        set(DOXYGEN_PROJECT_DESCRIPTION ${DESCRIPTION})
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Docs/Doxyfile)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target( ${NAME}_doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation for ${NAME}"
            VERBATIM
        )

        # Documentation installation
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
                DESTINATION ${CMAKE_INSTALL_DOCDIR}/${NAME}
        )
    else()
        message("🔴 Doxygen need to be installed to generate the doxygen documentation")
    endif()
endfunction()


# -----------------------------------------------------------------------------
# Function to create unitTest executable
# Use extra argument (ARGN) to Add dependendies only needed for the test executable
# -----------------------------------------------------------------------------

function(add_unit_test NAME )

    message("🟢 Add Unitest for ${NAME}")

    #List all test files
    file (GLOB_RECURSE  tests_SRC
           "${CMAKE_CURRENT_SOURCE_DIR}/unitTest/*.cpp"
           "${CMAKE_CURRENT_SOURCE_DIR}/unitTest/*.cxx"
    )

    # Add test executable target
    add_executable(${NAME}_test ${tests_SRC})

    # Find Google Test
    find_package(GTest REQUIRED)

    # Add test framework link libraries/dependencies
    target_link_libraries(
        ${NAME}_test PRIVATE ${NAME} GTest::gtest GTest::gtest_main ${ARGN}
    )

    # Add code coverage link libraries
    if(IS_LINUX)
        target_compile_options( ${NAME}_test PRIVATE -coverage  -fprofile-arcs -ftest-coverage)
        target_link_options( ${NAME}_test PRIVATE -coverage -lgcov)
    endif()

    # Automatically add tests with CTest
    gtest_discover_tests(
        ${NAME}_test
        XML_OUTPUT_DIR
        ./test_result/
        DISCOVERY_MODE
        PRE_TEST
    )

    # Test target dependencies
    add_dependencies(${NAME}_test ${NAME})

    #Check memory usage
    memcheck(${NAME}_test)
endfunction()


# -----------------------------------------------------------------------------
# Function to profile memory of a unit test (executable)
# -----------------------------------------------------------------------------
function(memcheck  UNIT_TEST)
    if(IS_LINUX AND BUILD_WITH_MEMCHECK_VAL)
        find_program(VALGRIND "valgrind")
        if (VALGRIND)
            add_custom_target(${UNIT_TEST}_memchecked
            COMMAND valgrind
            --error-exitcode=1
            --tool=memcheck
            --leak-check=full
            --show-reachable=yes
            --track-fds=yes
            --errors-for-leak-kinds=definite
            --show-leak-kinds=definite $<TARGET_FILE:${UNIT_TEST}>)
        else()
            message("🔴 Valgrind need to be installed to profile memory usage/leak")
        endif()
    else()
        #Dr memory from windows
    endif()
endfunction()


# additional target to perform clang-format run, requires clang-format

# get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
# foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
#     string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
#     if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
#         list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
#     endif ()
# # endforeach ()

# add_custom_target(
#         clangformat
#         COMMAND "C:/Users/Siebenou/Documents/Projects/C++/Tools/winlibs-x86_64-mcf-seh-gcc-13.2.0-llvm-16.0.6-mingw-w64ucrt-11.0.1-r2/mingw64/bin/clang-format.exe"
#         -style=LLVM
#         -i
#         ${ALL_SOURCE_FILES}
# )



# add_custom_target(
#         cppcheck
#         COMMAND /usr/bin/cppcheck
#         --enable=warning,performance,portability,information,missingInclude
#         --std=c++11
#         --library=qt.cfg
#         --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
#         --verbose
#         --quiet
#         ${ALL_SOURCE_FILES}
# )
