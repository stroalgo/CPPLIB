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
    -fdiagnostics-color=always # Enable colors in diagnostics
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
    -fdiagnostics-color=always # Enable colors in diagnostics
    -fdiagnostics-show-line-numbers # Show line numbers in diagnostics
    -fdiagnostics-show-note-include-stack # Show include stack in diagnostics
    -fshow-skipped-includes # Show skipped includes
    --save-stats # Save frontend statistics
    -fverify-intermediate-code # Verify the correctness of LLVM IR
    -fdebug-macro # Emit macros in debug info
    -fforce-dwarf-frame # Force emission of DWARF frame information
    -fstandalone-debug # Generate debug info for all types, even if unreferenced
    -fverbose-asm # Put extra commentary to the assembly output
    --verbose # Give verbose output about the build process
  )

  # ---------------------------------------------------------Compile Link
  # options--------------------------------------------------------
  add_link_options(
    -shared-libasan # AddressSanitizer
    -shared-libsan # AddressSanitizer, MemorySanitizer, ThreadSanitizer,
                   # UndefinedBehaviorSanitizer
    -fsanitize-link-c++-runtime # Link the C++ runtime with -fsanitize
    -fsanitize=address # Enable AddressSanitizer
  )

  # ---------------------------------------------------------Sanitize Compile
  # options--------------------------------------------------------
  add_compile_options(
    -fsanitize=address # Enable AddressSanitizer
    -fsanitize=undefined # Enable UndefinedBehaviorSanitizer
    -fsanitize=leak # Enable LeakSanitizer
    -fno-sanitize-ignorelist # Disable the use of the ignorelist for -fsanitize
    -fsanitize-address-poison-custom-array-cookie # Put a red zone after arrays
    -fsanitize-address-use-after-scope # Find uses of local variables outside
                                       # their scope allocated with new[]
    -fsanitize-address-use-after-return=always # Find uses of stack memory after
    # the containing function has returned
    -fsanitize=integer-divide-by-zero # Detect integer division by zero
    -fsanitize-cfi-cross-dso # Enable CFI protection across shared objects
    -fsanitize-stats # Collect and display sanitizer statistics
    # USE of memory sanitizer ==> -fsanitize=memory # -fmemory-profile # Enable
    # heap memory profiling -fsanitize-memory-param-retval # Run MemorySanitizer
    # on function parameters and return values -fsanitize-memory-track-origins #
    # Run MemorySanitizer with origin tracking USE of thread sanitizer ==>
    # -fsanitize=thread -fsanitize=thread -fcoroutines # Enable support for C++
    # coroutines ==> allow in c++20
  )

  message("🟢 CLANG DEBUG Compile options added")
endif()

# ---------------------------------------------------------Common Compile Link
# options--------------------------------------------------------
add_link_options(
  -shared-libgcc # Prevent issues with different libgcc versions
  -static-libstdc++ # Prevent issues with different libstdc++ versions
  -static-openmp # Link the OpenMP runtime statically
)

# ---------------------------------------------------------Common Compile
# options--------------------------------------------------------
add_compile_options(
  -Wdeprecated # Warn about the use of deprecated features
  -Werror # Make all warnings into errors
  -Wall # Enable most warning messages
  -Wextra # Enable extra warning messages
  -stdlib=libstdc++ # Use the LLVM implementation of the C++ standard library
  -fcheck-new # Check for failures of the new operator
  -ffinite-math-only # Assume that arguments and results of floating-point
                     # operations are not NaN or ±Inf
  -pipe # Use pipes rather than temporary files for communication between the
        # various stages of compilation
  -fstack-clash-protection # Generate code to check for stack clashes
  -pthread # Support multithreading with the pthreads library
  -finline-functions # Inline suitable functions
  -fpic # Position Independent Code, for shared libs
  -fbuiltin # Recognize built-in functions that do not begin with __builtin_
  -fcxx-exceptions # Enable C++ exception handling
  -fexceptions # Enable exception handling
  -fdelete-null-pointer-checks # Optimize under the assumption that
  # dereferencing a null pointer is undefined
  -ffinite-loops # Assume that loops will terminate
  -ffixed-point # Enable fixed-point arithmetic types and operations
  -fopenmp # Enable OpenMP support
  -fopenmp-extensions # Enable OpenMP extensions
  -fslp-vectorize # Enable superword-level parallelism vectorization
  -fvectorize # Enable vectorization
  -fstack-protector-all # Generate stack protection code for all functions
  -finline-hint-functions # Inline functions which are (explicitly or
  # implicitly) marked inline
  -fplt # Use the Procedure Linkage Table for function calls in shared libraries
)

# Try to locate clang's resource dir so AddressSanitizer runtime can be found at
# test runtime. If found, add it to the build RPATH and linker rpath so test
# executables can load libclang_rt.asan-*.so without requiring the user to set
# LD_LIBRARY_PATH manually.
execute_process(
  COMMAND "${CMAKE_C_COMPILER}" --print-resource-dir
  OUTPUT_VARIABLE CLANG_RESOURCE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
if(NOT CLANG_RESOURCE_DIR)
  # fallback to system clang if available
  execute_process(
    COMMAND clang --print-resource-dir
    OUTPUT_VARIABLE CLANG_RESOURCE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
endif()
if(CLANG_RESOURCE_DIR)
  set(CLANG_ASAN_DIR "${CLANG_RESOURCE_DIR}/lib/linux")
  if(EXISTS "${CLANG_ASAN_DIR}")
    message(STATUS "Found clang sanitizer runtime dir: ${CLANG_ASAN_DIR}")
    # Ensure build rpath contains the ASan runtime dir so built tests find it
    if(NOT DEFINED CMAKE_BUILD_RPATH)
      set(CMAKE_BUILD_RPATH
          "${CLANG_ASAN_DIR}"
          CACHE STRING "" FORCE)
    else()
      list(APPEND _TMP_RPATHS "${CMAKE_BUILD_RPATH}" "${CLANG_ASAN_DIR}")
      string(REPLACE ";" ":" _TMP_RPATH_JOINED "${_TMP_RPATHS}")
      set(CMAKE_BUILD_RPATH
          "${_TMP_RPATH_JOINED}"
          CACHE STRING "" FORCE)
    endif()

    # Add linker rpath flag so shared libraries/executables also get rpath
    add_link_options("-Wl,-rpath,${CLANG_ASAN_DIR}")

    # Export as LD_LIBRARY_PATH for immediate runs (helpful for CTest
    # invocation)
    if(DEFINED ENV{LD_LIBRARY_PATH})
      set(ENV{LD_LIBRARY_PATH} "${CLANG_ASAN_DIR}:$ENV{LD_LIBRARY_PATH}")
    else()
      set(ENV{LD_LIBRARY_PATH} "${CLANG_ASAN_DIR}")
    endif()
  else()
    message(
      VERBOSE
      "Clang resource dir found but ASan lib path does not exist: ${CLANG_ASAN_DIR}"
    )
  endif()
else()
  message(
    VERBOSE
    "Could not determine clang resource dir; ASan runtime rpath not added")
endif()
