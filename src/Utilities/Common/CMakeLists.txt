# -----------------------------------------------------------------------------
# NETWORK library project
# -----------------------------------------------------------------------------

# Project definitions
project(
    Common
    VERSION 1.0.0
    DESCRIPTION "This library contains utilies that are commonly use"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Boost REQUIRED )
find_package(OpenSSL REQUIRED )


# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
add_interface_library(${PROJECT_NAME} ${PROJECT_VERSION})

# Include directories
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE           
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
if(BUILD_TESTING)    
    add_unit_test(${PROJECT_NAME})    
endif()
