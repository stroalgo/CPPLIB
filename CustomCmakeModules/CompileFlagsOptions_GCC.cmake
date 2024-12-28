#---------------------------------------------------------Compile options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(
                -O3
                -fmodulo-sched
                -faggressive-loop-optimizations
                -flive-range-shrinkage
                -fselective-scheduling
                -fsel-sched-pipelining
                -fsel-sched-pipelining-outer-loops
                -fkeep-static-functions
                -fkeep-static-consts
                -fmerge-all-constants
                -fdelete-null-pointer-checks
                -fipa-pta
                -floop-parallelize-all
                -fivopts         
                -ftrivial-auto-var-init=zero   
                -flimit-function-alignment
                -ftracer
                -fstdarg-opt
                -fstack-clash-protection
                

                #### Available in GNU plateform
                #-fprofile-use
                #-fsection-anchors
        )
        message("GCC RELEASE options Flags added")
        
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")        
        add_compile_options(
                -g
                -ggdb
                -fvar-tracking
                -ginline-points
                -fno-eliminate-unused-debug-types
                -gvariable-location-views
                -ftrivial-auto-var-init=zero
                -fvtv-debug                            
                -fmem-report
                -fstack-usage
                -fstats
                # -Q  N.B: Build is too long
                
        )
        message("GCC RELEASE WITH DEBUG INFO options Flags added")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_compile_options(
                -g
                -ggdb
                -fvar-tracking
                -ginline-points
                -gstatement-frontiers
                -gvariable-location-views
                -fno-eliminate-unused-debug-types
                -fno-eliminate-unused-debug-symbols
                -O0
                -ftrivial-auto-var-init=zero
                -fprofile-abs-path                
                -fvtv-debug
                -freport-bug
                -fdump-earlydebug
                -fdump-passes
                -fmem-report
                -flto-report
                -fstack-usage
                -fstats         
                -fsanitize=pointer-overflow      

                # --coverage
                # -Q  N.B: Build is too long

                #### Available in GNU plateform
                #-pg
                #-p
        )
        message("GCC DEBUG options Flags added")
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(
        -pie
        -pthread
        -shared-libgcc 
        -ldl 
        -Wl,-Map=linker-map.map        
        -ffunction-sections
        -fdata-sections                
        -fsanitize=signed-integer-overflow
        -fsanitize=leak
        -fsanitize=null
        -fsanitize=vptr    
        -fsanitize=object-size
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fsanitize=nonnull-attribute
        -fsanitize=returns-nonnull-attribute
        -fsanitize=bool
        -fsanitize=enum
        -fsanitize=builtin
        -fsanitize-address-use-after-scope   
        -fsanitize-coverage=trace-cmp     
)  
        
#---------------------------------------------------------Common Compile options--------------------------------------------------------
add_compile_options(
    -Wall
    -Werror
    -Wextra
    -Wdangling-reference
    -Wdelete-non-virtual-dtor
    -Winvalid-constexpr
    -Winvalid-imported-macros
    -Wnoexcept
    -Wclass-memaccess
    -Wregister 
    -Wreorder
    -Wrange-loop-construct
    -Weffc++
    -Wstrict-null-sentinel
    -Wold-style-cast
    -Woverloaded-virtual
    -Wmismatched-new-delete
    -Wmismatched-tags
    -Wvolatile
    -Wzero-as-null-pointer-constant
    -Wcatch-value
    -Wsuggest-override
    -Wuseless-cast
    -Wfatal-errors
    -Wpedantic
    -Wnonnull
    -Winfinite-recursion
    -Winit-self
    -Wuninitialized 
    -Wmain
    -Wmisleading-indentation
    -Wmissing-attributes
    -Wswitch-default
    -Wunused-parameter
    -Wuninitialized
    -Wduplicated-branches
    -Wduplicated-cond
    -Wzero-length-bounds
    -Wtrampolines
    -Wfloat-equal
    -Wshadow
    -Wunsafe-loop-optimizations
    -Wundef
    -Wunused-macros
    -Wcast-qual
    -Wdangling-else
    -Wno-aggressive-loop-optimizations    
    -Wmissing-field-initializers
    -Wredundant-decls
    -Winvalid-pch
    -Wlong-long
    -fcheck-new
    -fstrict-enums
    -fstrong-eval-order    
    -fdiagnostics-show-location=every-line    
    -fstack-protector
    -fsanitize=shift
    -fsanitize=shift-exponent
    -fsanitize=shift-base
    -fsanitize=integer-divide-by-zero
    -fsanitize=bounds-strict
    -fsanitize=vla-bound
    -fsanitize=return
    -fsanitize=signed-integer-overflow
    -fsanitize=leak
    -fsanitize=null
    -fsanitize=vptr    
    -fsanitize=object-size
    -fsanitize=float-divide-by-zero
    -fsanitize=float-cast-overflow
    -fsanitize=nonnull-attribute
    -fsanitize=returns-nonnull-attribute
    -fsanitize=bool
    -fsanitize=enum
    -fsanitize=builtin
    -fsanitize-address-use-after-scope
    -fsanitize-coverage=trace-cmp
    -fsanitize=unreachable    
    -fcf-protection=full
    -fharden-compares
    -fharden-conditional-branches
    -fstack-protector-strong   
    -fexceptions
    -fdelete-dead-exceptions
    -ftrampolines
    
    #Precompiled Headers
#     -x 
#     -fpreprocessed
#     -fpch-preprocess
#     -H
)
