# -----------------------------------------------------------------------------
# NETWORK library project
# -----------------------------------------------------------------------------

# Project definitions
project(
    Network
    VERSION 1.0.0
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------
if(BUILD_DOC)
    add_doxygen_doc(${PROJECT_NAME})
endif()


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

# Link libraries
# target_link_libraries(
#     ${PROJECT_NAME}
#     PUBLIC)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------

