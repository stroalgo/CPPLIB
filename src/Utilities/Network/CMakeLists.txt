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
# Dependencies
# -----------------------------------------------------------------------------
find_package(Boost REQUIRED )
find_package(OpenSSL REQUIRED )


# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
add_shared_library(${PROJECT_NAME} ${PROJECT_VERSION})

# Add library sources
target_sources(
    ${PROJECT_NAME}
    PRIVATE sources/server.cpp)

# Include directories
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${Boost_INCLUDE_DIR}
        headers/       
)


# Link with other libraries
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        Boost::system
        Boost::filesystem
        openssl::openssl
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
