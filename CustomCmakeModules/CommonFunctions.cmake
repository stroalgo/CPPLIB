function(add_static_library NAME VERSION)
    # Add library target
    add_library(${NAME} STATIC)

    # Get SOVERSION
    string(
        REGEX MATCH
              "^([0-9]+)"
              SOVERSION
              ${VERSION}
    )

    # Set target version
    set_target_properties(
        ${NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SOVERSION}
    )    

    # Include directories
    target_include_directories(${NAME} PUBLIC inc)

    #Code Coverage
    if(CODE_COVERAGE)
        add_code_coverage(${NAME})
    endif()

    # # Install
    # install(
    #     TARGETS ${NAME}
    #     ARCHIVE
    #     LIBRARY
    #     RUNTIME
    # )

    # # Install header
    # install(FILES inc/${PROJECT_NAME}.hpp TYPE INCLUDE)
endfunction()


function(add_shared_library NAME VERSION)
    # Add library target
    add_library(${NAME} SHARED)

    # Get SOVERSION
    string(
        REGEX MATCH
              "^([0-9]+)"
              SOVERSION
              ${VERSION}
    )

    # Set target version
    set_target_properties(
        ${NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SOVERSION}
    )   

    # Include directories
    target_include_directories(${NAME} PUBLIC headers)

    #Code Coverage
    if(BUILD_WITH_CODE_COVERAGE)
        # add_code_coverage(${NAME})
    endif()

    # # Install
    # install(
    #     TARGETS ${NAME}
    #     ARCHIVE
    #     LIBRARY
    #     RUNTIME
    # )

    # # Install header
    # install(FILES inc/${PROJECT_NAME}.hpp TYPE INCLUDE)
endfunction()