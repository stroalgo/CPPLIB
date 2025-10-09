# ---------------------------------------------------------Compile
# options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(
    -O3 # Optimize for speed
    -ffast-math # Enable fast math optimizations
    -fstack-protector-strong # Generate extra code to check for buffer overflows
    -fstrict-aliasing # Assume the strictest aliasing rules
    -fstrict-enums # Assume that enumerations do not overflow
    -fstrict-overflow # Assume that signed arithmetic does not overflow
    -fstrict-return # Assume that functions with non-void return types always
                    # return a value
    -fstrict-vtable-pointers # Optimize based on the assumption that vtable
                             # pointers are not modified
    -flto=full # Number of partitions to use for parallel full LTO codegen
    -fvirtual-function-elimination # Enables dead virtual function elimination
                                   # optimization
    -fforce-emit-vtables # It causes more inline virtual functions to be
                         # emitted.
  )
  message("🟢 CLANG RELEASE Compile options added")

elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_compile_options(
    -g # Generate debug information
    -O3 # Optimize for speed
    -ffast-math # Enable fast math optimizations
    -ggdb3 # Debug level for GDB
    -gline-tables-only # Emit debug line number tables only
    -gmodules # Generate debug information for modules
    -fjmc # Enable just-my-code debugging
  )
  message("🟢 CLANG RELEASE WITH DEBUG INFO Compile options  added")

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(
    -g # Generate debug information
    -gfull # Generate maximum debug information
    -ggdb3 # Debug level for GDB
    -O0 # No optimization
    -gline-tables-only # Emit debug line number tables only
    -gmodules # Generate debug information for modules
    -fjmc # Enable just-my-code debugging
    -gcodeview # Generate debug information in CodeView format
    --analyze # Run the static analyzer
    -fshow-skipped-includes # Show skipped includes
    --save-stats # Save frontend statistics
    --verify-debug-info # Verify the binary representation of debug output
    -module-file-info # Display information about a module file
    -fverify-intermediate-code # Verify the correctness of LLVM IR
    --trace-includes # Trace header includes
    -fcolor-diagnostics # Enable colors in diagnostics
    -fdiagnostics-color=always # Enable colors in diagnostics
    -fdiagnostics-show-line-numbers # Show line numbers in diagnostics
    -fdiagnostics-show-note-include-stack # Show include stack in diagnostics
    -fdebug-macro # Emit macros in debug info
    -fforce-dwarf-frame # Force emission of DWARF frame information
    -fstandalone-debug # Generate debug info for all types, even if unreferenced
    -finstrument-functions # Generate instrumentation calls for entry and exit
                           # of functions
    -fverbose-asm # Put extra commentary to the assembly output
  )
  message("🟢 CLANG DEBUG Compile options added")
endif()

# ---------------------------------------------------------Compile Link
# options--------------------------------------------------------
add_link_options(
  -shared-libasan # AddressSanitizer
  -shared-libsan # AddressSanitizer, MemorySanitizer, ThreadSanitizer,
                 # UndefinedBehaviorSanitizer
  -shared-libgcc # Prevent issues with different libgcc versions
  -static-libstdc++ # Prevent issues with different libstdc++ versions
  -static-openmp # Link the OpenMP runtime statically
  -fsanitize-link-c++-runtime # Link the C++ runtime with -fsanitize
)
# ---------------------------------------------------------Common Compile
# options--------------------------------------------------------
add_compile_options(
  -Wdeprecated # Warn about the use of deprecated features
  -Werror # Make all warnings into errors
  -Wall # Enable most warning messages
  -Wextra # Enable extra warning messages
  -stdlib=libstdc++ # Use the LLVM implementation of the C++ standard library
  -dumpmachine # Display the compiler’s target processor
  -dumpversion # Display the version of the compiler
  -fcheck-new # Check for failures of the new operator
  -fcolor-diagnostics # Colorize the diagnostics
  -ffinite-math-only # Assume that arguments and results of floating-point
                     # operations are not NaN or ±Inf
  -pipe # Use pipes rather than temporary files for communication between the
        # various stages of compilation
  -fstack-clash-protection # Generate code to check for stack clashes
  -pthread # Support multithreading with the pthreads library
  -time # Time individual commands
  --verbose # Give verbose output about the build process
  -finline-functions # Inline suitable functions
  -fno-sanitize-ignorelist # Disable the use of the ignorelist for -fsanitize
  -fsanitize-address-poison-custom-array-cookie # Put a red zone after arrays
                                                # allocated with new[]
  -fsanitize-address-use-after-scope # Find uses of local variables outside
                                     # their scope
  -fsanitize-address-use-after-return=always # Find uses of stack memory after
                                             # the containing function has
                                             # returned
  -fsanitize=integer-divide-by-zero # Detect integer division by zero
  -fsanitize-cfi-cross-dso # Enable CFI protection across shared objects
  -fsanitize-memory-track-origins # Run MemorySanitizer with origin tracking
  -fsanitize-stats # Collect and display sanitizer statistics
  -fpic # Position Independent Code, for shared libs
  -fpie # Position Independent Executable, for executables
  -fbuiltin # Recognize built-in functions that do not begin with __builtin_
  -fchar8_t # Support the char8_t type as introduced in C++20
  -fcoroutines # Enable support for C++ coroutines
  -fcxx-exceptions # Enable C++ exception handling
  -fexceptions # Enable exception handling
  -fdelete-null-pointer-checks # Optimize under the assumption that
                               # dereferencing a null pointer is undefined
  -ffinite-loops # Assume that loops will terminate
  -ffixed-point # Enable fixed-point arithmetic types and operations
  -fmemory-profile # Enable heap memory profiling
  -fmodules-search-all # Search all module paths for imports
  -fopenmp # Enable OpenMP support
  -fopenmp-extensions # Enable OpenMP extensions
  -fsanitize-memory-param-retval # Run MemorySanitizer on function parameters
                                 # and return values
  -fslp-vectorize # Enable superword-level parallelism vectorization
  -fvectorize # Enable vectorization
  -fstack-protector-all # Generate stack protection code for all functions
  -finline-hint-functions # Inline functions which are (explicitly or
                          # implicitly) marked inline
  -fsanitize=thread # Enable ThreadSanitizer
  -fplt # Use the Procedure Linkage Table for function calls in shared libraries
)
