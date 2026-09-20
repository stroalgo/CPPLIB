# -----------------------------------------------------------------------------
# COMMON library project
# -----------------------------------------------------------------------------

# Project definitions
project(
  Common
  VERSION 1.0.0
  DESCRIPTION "This library contains utilities that are commonly used"
  LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Build Library
# -----------------------------------------------------------------------------
add_shared_library(${PROJECT_NAME} ${PROJECT_VERSION})

# Sources Files
target_sources(${PROJECT_NAME} PRIVATE sources/Utils.cpp)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC headers/)
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
  set(DEPENDENCIES Boost::filesystem Boost::system)
  add_unit_test(${PROJECT_NAME} ${DEPENDENCIES})
endif()
