#---------------------------------------------------------Compile options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(
                /Ot
                /O2
                /Qpar
                /GA
                /GL
                /Qfast_transcendentals
        )
        
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")        
        add_compile_options(
                /GA
                /O1
                /Os
                /Zi           
      )

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(
                /diagnostics:column
                /analyze
                /FC
                /FS
                /showIncludes
                /Zf
                /Zi
                /RTCs
                /RTC1
        )
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(
        /MD
        /MT
        /LN
        /LD
)  
        
#---------------------------------------------------------Common Compile options--------------------------------------------------------
add_compile_options(

/Wall
/WX
/sdl
/options:strict
/external:anglebrackets
/MP
/utf-8
/await
/translateInclude
/Fo
/EHa
/EHr
/fsanitize=address
/GS
/guard:cf
)

