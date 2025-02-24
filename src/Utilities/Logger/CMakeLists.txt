# -----------------------------------------------------------------------------
# LOGGER library project
# -----------------------------------------------------------------------------

# Project definitions
project(
    Logger
    VERSION 1.0.0
    DESCRIPTION "This library contains Logger utilities"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
add_shared_library(${PROJECT_NAME} ${PROJECT_VERSION})

# Sources Files
target_sources(
    ${PROJECT_NAME}
    PRIVATE sources/Logger.cpp)

# Include directories
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC        
        headers/          
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        spdlog::spdlog           
        Common
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
    set(DEPENDENCIES Boost::date_time)
    add_unit_test(${PROJECT_NAME} ${DEPENDENCIES})    
endif()
