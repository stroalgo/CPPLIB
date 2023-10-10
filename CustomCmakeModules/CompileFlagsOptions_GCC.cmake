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
                # -fprofile-use
                -ftracer
                -fstdarg-opt
                # -fsection-anchors
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
                -Q
                -fmem-report
                -fstack-usage
                -fstats
                -dumpmachine
                -dumpfullversion

        )
        message("GCC RELEASE WITH DEBUG INFO options Flags added")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

        add_compile_options(
                # -g
                # -ggdb
                # -fvar-tracking
                # -ginline-points
                # -gstatement-frontiers
                # -gvariable-location-views
                # -fno-eliminate-unused-debug-types
                # -fno-eliminate-unused-debug-symbols
                # -O0
                # -ftrivial-auto-var-init=zero
                # # -pg
                # # -p
                # # --coverage
                # -fprofile-abs-path
                # # -fsanitize=kernel-hwaddress
                # # -fsanitize=shadow-call-stack
                # -fvtv-debug
                # -freport-bug
                # -fdump-earlydebug
                # -fdump-passes
                # -Q
                # -fmem-report
                # -flto-report
                # -fstack-usage
                # -fstats         
                -fsanitize=pointer-overflow      
        )
        message("GCC DEBUG options Flags added")
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(

        -pie
        -pthread
        -shared-libgcc
        # -symbolic
        # -Wl,–verbose
        # -Wl,–print-memory-usage
        -ldl 
        -Wl,-Map=linker-map.map
        # -Wl,–gc-sections
        -ffunction-sections
        -fdata-sections
        
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
    # -fsyntax-only
    -fstack-protector
#     -fsanitize=undefined  N.B: undefined reference to `__ubsan_handle_type_mismatch_v1'
    -fsanitize=shift
    -fsanitize=shift-exponent
    -fsanitize=shift-base
    -fsanitize=integer-divide-by-zero
    -fsanitize=unreachable
    -fsanitize=vla-bound
#     -fsanitize=null  N.B: undefined reference to `__ubsan_handle_type_mismatch_v1'
    -fsanitize=return
    -fsanitize=signed-integer-overflow
    -fsanitize=bounds-strict
#     -fsanitize=alignment  N.B: undefined reference to `__ubsan_handle_type_mismatch_v1'
    -fsanitize=object-size
    -fsanitize=float-divide-by-zero
    -fsanitize=float-cast-overflow
    -fsanitize=nonnull-attribute
    -fsanitize=returns-nonnull-attribute
    -fsanitize=bool
    -fsanitize=enum
#     -fsanitize=vptr  N.B: undefined reference to `__ubsan_handle_dynamic_type_cache_miss'
    
    -fsanitize=builtin
    -fsanitize-address-use-after-scope
#     -fsanitize-coverage=trace-pc
    -fsanitize-coverage=trace-cmp
    -fcf-protection=full
    -fharden-compares
    -fharden-conditional-branches
    -fstack-protector-strong
    -fstack-check
#     -fstack-clash-protection
#     -fvtable-verify=std   N.B: undefined reference to `__VLTRegisterPair'
#     -finstrument-functions-once  N.B: undefined reference to `__sanitizer_cov_trace_const_cmp1'
    -fexceptions
    -fdelete-dead-exceptions
    -ftrampolines
    #Precompiled Headers
#     -x 
#     -fpreprocessed
#     -fpch-preprocess
#     -H

)