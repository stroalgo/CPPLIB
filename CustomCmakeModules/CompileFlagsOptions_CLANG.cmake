#---------------------------------------------------------Compile options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(
        -fstrict-aliasing
        -fstrict-enums
        -fstrict-overflow
        -fstrict-return
        -fstrict-vtable-pointers
        -flto=full
        -fvirtual-function-elimination
        -fwhole-program-vtables
        -Ofast3
        -fforce-emit-vtables    
                
        )
        message("CLANG RELEASE Compile options added")
        
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")        
        add_compile_options(
                -g
                -Ofast3
                -ggdb3
        )
        message("🟢 CLANG RELEASE WITH DEBUG INFO Compile options  added")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(
                -Weverything
                -gcodeview
                -g
                -gfull
                -ggdb3
                -gmodules
                -ggdb
                -fcolor-diagnostics
                -fdiagnostics-color                
                -fstandalone-debug
                -fdiagnostics-format=clang                
        )
        message("🟢 CLANG DEBUG Compile options added")
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(
        -shared-libgcc
        -shared-libsan
        -static-libstdc++
        -static-openmp
        # --coverage        
        )  
        
#---------------------------------------------------------Common Compile options--------------------------------------------------------
add_compile_options(
    -Wall
    -Wambiguous-member-template
    -Werror
    -pipe
    -pthread
    --verbose
    -finline-functions
    -finline-hint-functions
    -fsanitize-cfi-cross-dso
    -fsanitize-link-c++-runtime
    -fsanitize=undefined
    -fsanitize-stats
    -Wdeprecated
    -fcxx-exceptions
    -ffinite-math-only
    -ffixed-point
    -fms-extensions 
    -fpic
    -fpie
    -fplt    
    -fstack-protector-all
    -fvectorize    
    -Wno-documentation-unknown-command
    ##### Available in GNU plateform
    #     -fsanitize=dataflow    
    #     -finstrument-functions
    #     -fsanitize=memory
)