#---------------------------------------------------------Choose Compiler : Default is GCC--------------------------------------------------------
option(BUILD_WITH_CLANG          "Build with Clang Compiler"     OFF)
option(BUILD_WITH_MSCV           "Build with MSVC Compiler"      ON)

if(BUILD_WITH_CLANG)
    set(ROOT_TOOLCHAIN_PATH "C:/Users/Siebenou/Documents/Projects/C++/Tools/winlibs-x86_64-mcf-seh-gcc-13.2.0-llvm-16.0.6-mingw-w64ucrt-11.0.1-r2/mingw64/bin")
    set(CMAKE_C_COMPILER                "${ROOT_TOOLCHAIN_PATH}/clang.exe")
    set(CMAKE_CXX_COMPILER              "${ROOT_TOOLCHAIN_PATH}/clang++.exe")
    #Compilation flags
    include(CompileFlagsOptions_CLANG)
    message("Using CLANG as compiler")
elseif(BUILD_WITH_MSCV)
    set(ROOT_TOOLCHAIN_PATH "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x64")
    set(CMAKE_C_COMPILER                "${ROOT_TOOLCHAIN_PATH}/cl.exe")
    set(CMAKE_CXX_COMPILER              "${ROOT_TOOLCHAIN_PATH}/cl.exe")
    #Compilation flags
    include(CompileFlagsOptions_MSVC)
    message("Using MSVC as compiler")
else()
    set(ROOT_TOOLCHAIN_PATH "C:/Users/Siebenou/Documents/Projects/C++/Tools/winlibs-x86_64-mcf-seh-gcc-13.2.0-llvm-16.0.6-mingw-w64ucrt-11.0.1-r2/mingw64/bin")
    set(CMAKE_C_COMPILER                "${ROOT_TOOLCHAIN_PATH}/gcc.exe")
    set(CMAKE_CXX_COMPILER              "${ROOT_TOOLCHAIN_PATH}/g++.exe")
    #Compilation flags
    include(CompileFlagsOptions_GCC)
    message("Using GCC as compiler")
endif()

set(CMAKE_SYSTEM_NAME               Windows)
set(CMAKE_SYSTEM_PROCESSOR          x86_64)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)