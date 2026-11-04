# learnsfml_add_game(<name>)
#
# Builds games/<name>/src/**.cpp as an executable. Public headers live in
# include/, private headers in src/. Links SFML and the shared common library,
# and writes the binary to bin/<platform>/<config>.
function(learnsfml_add_game game_name)
    file(GLOB_RECURSE _sources CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
    file(GLOB_RECURSE _private_headers CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
    file(GLOB_RECURSE _public_headers CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
         "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

    if(NOT _sources)
        message(FATAL_ERROR
            "learnsfml_add_game(${game_name}): no sources under "
            "${CMAKE_CURRENT_SOURCE_DIR}/src")
    endif()

    add_executable(${game_name}
        ${_sources} ${_private_headers} ${_public_headers})

    # include/ is public (exported), src/ is private (not exported).
    target_include_directories(${game_name}
        PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
    target_link_libraries(${game_name} PRIVATE SFML::Graphics learnsfml::common)

    set_target_properties(${game_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${LEARNSFML_BIN_DIR}")

    # In shared builds, put SFML's DLLs next to the exe so it can start as-is.
    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET ${game_name} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                    $<TARGET_RUNTIME_DLLS:${game_name}>
                    $<TARGET_FILE_DIR:${game_name}>
            COMMAND_EXPAND_LISTS
            VERBATIM)
    endif()
endfunction()
