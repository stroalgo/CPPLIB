# ---------------------------------------------------------Compile
# options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(
    -O3 # maximize optimization for speed
    -fmodulo-sched # enable modulo scheduling for loops (improve loop
                   # throughput)
    -faggressive-loop-optimizations # more aggressive loop transformations
    -flive-range-shrinkage # reduce live ranges to enable more register reuse
    -fselective-scheduling # enable selective scheduling heuristics
    -fsel-sched-pipelining # enable software pipelining of loops
    -fsel-sched-pipelining-outer-loops # pipeline outer loops as well
    -fkeep-static-functions # keep static functions from being discarded
                            # (affects linkage)
    -fkeep-static-consts # keep static consts (avoid folding/removal)
    -fmerge-all-constants # merge duplicate constants to reduce code size
    -fdelete-null-pointer-checks # remove redundant null-pointer checks
    -fipa-pta # interprocedural points-to analysis (improves interprocedural
              # optimizations)
    -floop-parallelize-all # consider parallelizing loops automatically
    -fivopts # improve induction variable optimizations
    -ftrivial-auto-var-init=zero # initialize automatic vars to zero to avoid
                                 # undefined data
    -flimit-function-alignment # limit function alignment to reduce code
                               # size/over-alignment
    -ftracer # instrumentation for tracer hooks (GCC internal)
    -fstdarg-opt # optimize varargs usage
    -fstack-clash-protection # mitigate stack clash attacks
    # Available in GNU plateform -fprofile-use            # use profile data to
    # guide optimizations (requires .gcda/.gcno) -fsection-anchors       # keep
    # section anchors to help with dead-stripping
  )
  message("🟢 GCC RELEASE options Flags added")

elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_compile_options(
    -g # generate debug information
    -ggdb # generate GDB-friendly debug info
    -fvar-tracking # improve variable tracking for better debugging
    -ginline-points # better inline debug info (where inlined funcs correspond)
    -fno-eliminate-unused-debug-types # keep unused debug types for accurate
                                      # debugging
    -gvariable-location-views # improved debug info for variable locations
    -ftrivial-auto-var-init=zero # initialize automatic vars to zero for defined
                                 # behavior
    -fvtv-debug # enable virtual table verification debugging support
    -fmem-report # emit memory usage report (GCC-specific)
    -fstack-usage # produce stack usage info per function
    -fstats # emit compiler statistics
    # -Q  N.B: Build is too long
  )
  message("🟢 GCC RELEASE WITH DEBUG INFO options Flags added")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")

  add_compile_options(
    -g # generate debug information
    -ggdb # GDB-friendly debug info
    -fvar-tracking # improve variable tracking for debugger
    -ginline-points # inline point debug info
    -gstatement-frontiers # improve statement-level debugging boundaries
    -gvariable-location-views # better variable location info for debuggers
    -fno-eliminate-unused-debug-types # keep debug types even if unused
    -fno-eliminate-unused-debug-symbols # keep debug symbols even if unused
    -O0 # disable optimizations for easier debugging
    -ftrivial-auto-var-init=zero # init automatic vars to avoid uninitialized
                                 # reads
    -fprofile-abs-path # use absolute paths in profiling/debug output
    -fvtv-debug # vtable verification debug
    -freport-bug # include extra info to help report compiler bugs
    -fdump-earlydebug # dump early debug info for compiler debugging
    -fdump-passes # dump compiler passes information
    -fmem-report # memory usage reporting
    -flto-report # link-time optimization report
    -fstack-usage # report stack usage per function
    -fstats # emit compiler stats
    -fsanitize=pointer-overflow # detect pointer arithmetic overflow
    # --coverage -Q  N.B: Build is too long Available in GNU plateform -pg #
    # compile for gprof profiling -p                       # compile for basic
    # profiling
  )
  message("🟢 GCC DEBUG options Flags added")
endif()

# ---------------------------------------------------------Common Sanitizer
# Flags--------------------------------------------------------
set(COMMON_SANITIZER_FLAGS
    -fsanitize=signed-integer-overflow # link-time sanitizer for signed integer
                                       # overflow
    -fsanitize=null # detect null dereferences
    -fsanitize=vptr # detect invalid virtual pointer uses (C++ vtables)
    -fsanitize=object-size # check object size-related errors
    -fsanitize=float-divide-by-zero # detect floating-point divide-by-zero
    -fsanitize=float-cast-overflow # detect floating-point cast overflow
    -fsanitize=nonnull-attribute # check nonnull attribute violations
    -fsanitize=returns-nonnull-attribute # check functions marked
                                         # returns_nonnull
    -fsanitize=bool # boolean sanitizer checks
    -fsanitize=enum # enum sanitizer checks
    -fsanitize=builtin # builtin function checks
    -fsanitize-address-use-after-scope # detect use-after-scope for local
                                       # variables
    -fsanitize-coverage=trace-cmp # enable sanitizer coverage for comparison
                                  # tracing
)

# ---------------------------------------------------------Compile Link
# options--------------------------------------------------------
add_link_options(
  -pie # produce position independent executable
  -pthread # link with pthreads (thread support)
  -shared-libgcc # use shared libgcc (reduce static linking of libgcc)
  -ldl # link libdl for dynamic loading
  -Wl,-Map=linker-map.map # instruct linker to generate a map file
  -ffunction-sections # put each function in its own section (enables
                      # dead-stripping)
  -fdata-sections # put each data item in its own section
  ${COMMON_SANITIZER_FLAGS})

# ---------------------------------------------------------Common Compile
# options--------------------------------------------------------
add_compile_options(
  -Wall # enable commonly-used warnings
  -Werror # treat warnings as errors
  -Wextra # enable extra warnings
  -Wdangling-reference # warn about dangling references
  -Wdelete-non-virtual-dtor # warn when deleting through base with non-virtual
                            # dtor
  -Winvalid-constexpr # warn about invalid constexpr usage
  -Winvalid-imported-macros # warn about invalid imported macros
  -Wnoexcept # warn about noexcept issues
  -Wclass-memaccess # warn about suspicious class member memory access
  -Wregister # warn about use of register storage-class specifier (deprecated)
  -Wreorder # warn about reorder of initializers in constructors
  -Wrange-loop-construct # warn about range-based for issues
  -Weffc++ # enable Effective C++ warnings (heuristics)
  -Wstrict-null-sentinel # strict null sentinel checks
  -Wold-style-cast # warn on old-style C casts
  -Woverloaded-virtual # warn when overloaded virtual functions hide base ones
  -Wmismatched-new-delete # warn on mismatched new/delete forms
  -Wmismatched-tags # warn on mismatched struct/union/class tags
  -Wvolatile # warn on volatile issues
  -Wzero-as-null-pointer-constant # warn when 0 used as null pointer constant
  -Wcatch-value # warn when exceptions are caught by value
  -Wsuggest-override # suggest adding override keyword
  -Wuseless-cast # warn on casts that have no effect
  -Wfatal-errors # stop compilation on first error (treat as fatal)
  -Wpedantic # strict ISO compliance warnings
  -Wnonnull # warn about null dereference on nonnull parameters
  -Winfinite-recursion # warn on obvious infinite recursion
  -Winit-self # warn about self-initialization
  -Wuninitialized # warn about uninitialized variables
  -Wmain # warn about suspicious main signatures
  -Wmisleading-indentation # warn about misleading indentation
  -Wmissing-attributes # warn about missing function/variable attributes
  -Wswitch-default # warn if switch has no default label
  -Wunused-parameter # warn about unused function parameters
  -Wuninitialized # duplicate: warn about uninitialized variables
  -Wduplicated-branches # warn about duplicated branches in conditionals
  -Wduplicated-cond # warn about duplicated conditions
  -Wzero-length-bounds # warn when bounds are length zero
  -Wtrampolines # warn about trampolines (nested function trampolines)
  -Wfloat-equal # warn about comparing floats for equality
  -Wshadow # warn about variable shadowing
  -Wunsafe-loop-optimizations # warn about optimizations that may change loop
                              # behavior
  -Wundef # warn about undefined macros in #if
  -Wunused-macros # warn about unused macros
  -Wcast-qual # warn about casts that discard qualifiers
  -Wdangling-else # warn about ambiguous dangling else
  -Wno-aggressive-loop-optimizations # disable aggressive loop optimization
                                     # warnings (negation)
  -Wmissing-field-initializers # warn when fields in aggregate are not
                               # initialized
  -Wredundant-decls # warn about redundant declarations
  -Winvalid-pch # warn about invalid precompiled headers
  -Wlong-long # warn about use of long long (if deprecated)
  -fcheck-new # run-time checks for new (throw on allocation failure)
  -fstrict-enums # assume enums have only listed values (improves optimizations)
  -fstrong-eval-order # enforce stronger evaluation order for expressions
  -fdiagnostics-show-location=every-line # show location for every diagnostic
                                         # line
  -fdiagnostics-show-option # show which option enabled a diagnostic (useful for
                            # analysis)
  -fstack-protector # protect against stack smashing
  -fcf-protection=full # enable control flow protection (spectre/ret2
  # mitigations)
  -fharden-compares # harden integer comparisons against overflow attacks
  -fharden-conditional-branches # harden conditional branches
  -fstack-protector-strong # stronger stack protector (more functions protected)
  -fexceptions # enable exception handling
  -fdelete-dead-exceptions # remove exceptions that are provably dead
  -ftrampolines # allow trampolines for nested functions
  -fsanitize=shift # detect undefined shift operations
  -fsanitize=shift-exponent # detect invalid shift exponent
  -fsanitize=shift-base # detect invalid shift base
  -fsanitize=integer-divide-by-zero # detect integer divide-by-zero
  -fsanitize=bounds-strict # strict bounds checking
  -fsanitize=vla-bound # detect variable length array bound issues
  -fsanitize=return # check for returning address of local object
  -fsanitize=unreachable # detect unreachable code being executed
  ${COMMON_SANITIZER_FLAGS})

# ---------------------------------------------------------Memory/Leak
# Profiling--------------------------------------------------------
if(NOT BUILD_WITH_MEMCHECK_VAL)
  add_link_options(-fsanitize=leak # enable leak sanitizer at link-time
  )
  add_compile_options(-fsanitize=leak # enable leak sanitizer at compile-time
  )
  message("🟢 ASAN G++ built-in is used for memory/leak profiling")
else()
  message("🟢 VALGRIND is used for memory/leak profiling")
endif()
