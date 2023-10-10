cmake_minimum_required(VERSION 3.27.2)

#---------------------------------------------------------Project Settings--------------------------------------------------------
project(CppLib 
        VERSION 1.0.0
        LANGUAGES CXX)

#---------------------------------------------------------Additional Modules--------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CustomCmakeModules")
include(CommonFunctions)


#---------------------------------------------------------Options--------------------------------------------------------
option(BUILD_WITH_TEST          "Build units tests"      ON)
option(BUILD_WITH_DOC           "Generate Documentation" OFF)
option(BUILD_WITH_CODE_COVERAGE "Enable Code Coverage"   OFF)
set(GCC_COLORS "error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01")

#---------------------------------------------------------Language Settings--------------------------------------------------------
#C Settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

#C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#Compilation flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    include(CompileFlagsOptions_GCC)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    include(CompileFlagsOptions_CLANG)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    include(CompileFlagsOptions_MSVC)
else()
    message(FATAL_ERROR "This compiler is not yet supported : ${CMAKE_CXX_COMPILER_ID}")
endif()


#---------------------------------------------------------Unit Test Settings--------------------------------------------------------
if(BUILD_WITH_TEST)
    include(CTest)
    enable_testing()
endif()

#--------------------------------------------------------- Add SubDir--------------------------------------------------------
add_subdirectory(Utilities)







