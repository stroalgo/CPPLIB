#---------------------------------------------------------Compile options--------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(
            
        )
        
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")        
        add_compile_options(

        )

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(

        )
endif()       
        
#---------------------------------------------------------Compile Link options--------------------------------------------------------
add_link_options(

)  
        
#---------------------------------------------------------Common Compile options--------------------------------------------------------
add_compile_options(
    /Wall
    /Werror

)

