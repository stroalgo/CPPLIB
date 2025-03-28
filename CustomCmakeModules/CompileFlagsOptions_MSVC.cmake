#---------------------------------------------------------Compile options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(
                /GA                     #Optimizes for Windows applications.
                /O2                     #Creates fast code
                /Ob1                    # Controls inline expansion
                /Ot                     #Favors fast code
                /Os                     #Favors small code     	
                /Oi                     #Replaces some function calls with intrinsic that help your application run faster.
                /fp:fast                #option allows the compiler to reorder, combine, or simplify floating-point operations to optimize floating-point code for speed and space.
                /Qfast_transcendentals  #option forces transcendental functions to be converted to inline code to improve execution speed
        )

        add_link_options(
                /RELEASE        #Sets the Checksum in the .exe header.
                /MD             #Compiles to create a multithreaded DLL
        )
        
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")        
        add_compile_options(
                /GA #Optimizes for Windows applications.
                /O1 #Creates small code
                /O2 #Creates fast code
                /Os #Favors small code     	
                /Zi #Generates complete debugging information.
      )

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(
                /fp:strict              #the compiler generates code that allows the program to safely unmask floating-point exceptions, read or write floating-point status registers, or change rounding modes
                /GR                     #Adds code to check object types at run time.
                /guard:cf               #option causes the compiler to analyze control flow for indirect call targets at compile time
                /Zi                     #Generates complete debugging information.
                /analyze                #Turns on code analysis.
                /diagnostics:column     #option also includes the column where the issue was found
                /RTCc                 #Reports when a value is assigned to a smaller data type and results in a data loss.
                /RTCs                   #Enables stack frame run-time error checking
                /RTCu                   #Reports when a variable is used without having been initialized.
                /RTC1                   #Enable fast runtime checks
                /Fo                     #Creates an object file.           
                /FC                     #Displays the full path of source code files passed to cl.exe in diagnostic text.
                /Zf                     #Improves PDB generation time in parallel builds.                
                /showIncludes           #Displays a list of all include files during compilation.             
        )

        add_link_options(
                /PROFILE        #Produces an output file that can be used with the Performance Tools profiler.
                /LDd            #Creates a debug dynamic-link library.  
                /MDd            #Compiles to create a debug multithreaded DLL                
        )
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(        
        /WX             #Treats linker warnings as errors.
        /MACHINE:x64    #Specifies the target platform.
        /APPCONTAINER   #Specifies whether the app must run within an appcontainer process environment.       
)  
        
#---------------------------------------------------------Common Compile options--------------------------------------------------------
add_compile_options(
        # /await #Option enables compiler support for C++ coroutines
        /WX                                     #Treats all compiler warnings as errors.
        /external:anglebrackets                 #Treat all headers included via <> as external.
        /WL                                     #Enable one-line diagnostics for error and warning messages
        /options:strict                         #Unrecognized compiler options are errors.
        /EHa                                    #Enable C++ exception handling (with SEH exceptions).
        /GF                                     #Optimization called string pooling that can create smaller programs.
        /GS                                     #Detects some buffer overruns that overwrite a function's return address
        /GT                                     #Supports fiber safety for data allocated using static thread-local storage
        /Gu                                     #Ensure distinct functions have distinct addresses.
        /Gw                                     #Option causes the compiler to package global data in individual COMDAT sections
        /Qpar                                   #Enables automatic parallelization of loops.
        /Zc:__cplusplus                         #option enables the __cplusplus preprocessor macro
        /Zc:alignedNew                          #Enable support for C++17 over-aligned new
        /Zc:enumTypes                           #option implements Standard C++ conforming behavior for deduction of enumeration base types
        /Zc:lambda                              #compiler option enables the conforming lambda processor
        /sdl                                    #Enable more security features and warnings.
        /utf-8                                  #Set source and execution character sets to UTF-8.
        /MP                                     #Builds multiple source files concurrently.
        /D_WIN32_WINNT=0x0A00                   #It defines the version of the windows header files to use(In this case Win10)
        /fsanitize-address-use-after-return     #a compiler instrumentation of used after return
)