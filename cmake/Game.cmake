# learnsfml_add_game(<name>)
#
# Builds games/<name>/src/** as an executable, links it to SFML and the shared
# common library, and writes the binary to bin/<platform>/<config>.
function(learnsfml_add_game game_name)
    file(GLOB_RECURSE _sources CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

    if(NOT _sources)
        message(FATAL_ERROR
            "learnsfml_add_game(${game_name}): no sources under "
            "${CMAKE_CURRENT_SOURCE_DIR}/src")
    endif()

    add_executable(${game_name} ${_sources})

    # common exports its own public include dirs, so games only link the module.
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
