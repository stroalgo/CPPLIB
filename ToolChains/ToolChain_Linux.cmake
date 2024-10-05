#---------------------------------------------------------Choose Compiler : Default is GCC--------------------------------------------------------
option(BUILD_WITH_CLANG          "Build with Clang Compiler"     OFF)


set(ROOT_TOOLCHAIN_PATH "/usr/bin")
if(BUILD_WITH_CLANG)
    set(CMAKE_C_COMPILER                "${ROOT_TOOLCHAIN_PATH}/clang")
    set(CMAKE_CXX_COMPILER              "${ROOT_TOOLCHAIN_PATH}/clang++")
    #Compilation flags
    include(CompileFlagsOptions_CLANG)
    message("🟢 Using CLANG as compiler")
else()
    set(CMAKE_C_COMPILER                "${ROOT_TOOLCHAIN_PATH}/gcc")
    set(CMAKE_CXX_COMPILER              "${ROOT_TOOLCHAIN_PATH}/g++")
    #Compilation flags
    include(CompileFlagsOptions_GCC)
    message("🟢 Using GCC as compiler")
endif()
set(CMAKE_SYSTEM_NAME               Linux)
set(CMAKE_SYSTEM_PROCESSOR          x86_64)


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 