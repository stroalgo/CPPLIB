# -----------------------------------------------------------------------------
# NETWORK library project
# -----------------------------------------------------------------------------

# Project definitions
project(
    Network
    VERSION 1.0.0
    DESCRIPTION "Network project description"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
add_shared_library(${PROJECT_NAME} ${PROJECT_VERSION})

# Add library sources
target_sources(
    ${PROJECT_NAME}
    PRIVATE sources/test.cpp)

# Include directories
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        headers/       
)

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------
if(BUILD_WITH_DOC)
    make_doxygen_doc(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_DESCRIPTION})
endif()


# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------

