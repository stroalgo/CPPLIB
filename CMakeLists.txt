cmake_minimum_required(VERSION 3.26.4)

#Apply Out-of-Source Builds Only
if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "🔴 In-source builds are not allowed.Create a subfolder and use `cmake ..` inside it.")
endif()

#---------------------------------------------------------Additional Modules--------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CustomCmakeModules")
include(CommonFunctions)




#---------------------------------------------------------ToolChain Settings--------------------------------------------------------
##TODO : Manage flags with conan
# if (CMAKE_HOST_SYSTEM_NAME  STREQUAL "Linux")
#     message("🟢 Target platform : LINUX")
#     set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ToolChains/ToolChain_Linux.cmake")
# elseif(CMAKE_HOST_SYSTEM_NAME  STREQUAL "Windows")
#     message("🟢 Target platform : WINDOWS")
#     set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ToolChains/ToolChain_Win.cmake")
# else()
#     message(FATAL_ERROR "🔴 This platform is not yet supported : ${CMAKE_HOST_SYSTEM_NAME}")
# endif()

#---------------------------------------------------------Project Settings--------------------------------------------------------
project(CppLib 
        VERSION 1.0.0
        LANGUAGES CXX)    
        
        
#---------------------------------------------------------Additional CMake modules--------------------------------------------------------
include(GNUInstallDirs)

#to ON for  creating compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#---------------------------------------------------------Options--------------------------------------------------------
option(BUILD_WITH_TEST          "Build units tests"      ON)
option(BUILD_WITH_DOC           "Generate Documentation" ON)
option(BUILD_WITH_CODE_COVERAGE "Enable Code Coverage"   OFF)

#---------------------------------------------------------Language/Build--------------------------------------------------------
#C Settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

#C++ Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#Project Install
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/_DELIVERY_/${CMAKE_BUILD_TYPE}" CACHE PATH "install prefix" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#---------------------------------------------------------Unit Test Settings--------------------------------------------------------
if(BUILD_WITH_TEST)
    include(CTest)
    include(GoogleTest)
    enable_testing()
endif()

#--------------------------------------------------------- Add SubDir--------------------------------------------------------
add_subdirectory(src)


#--------------------------------------------------------- Package--------------------------------------------------------
include(Package)
include(CPack)






