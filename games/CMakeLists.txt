# Auto-discover each game folder that contains a CMakeLists.txt, so adding a
# new game (e.g. via scripts/new_game.py) needs no edits here.
# CONFIGURE_DEPENDS makes CMake re-glob automatically when a folder appears.
file(GLOB _learnsfml_game_dirs
     LIST_DIRECTORIES true
     RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
     CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(_dir IN LISTS _learnsfml_game_dirs)
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}"
       AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_dir}/CMakeLists.txt")
        add_subdirectory("${_dir}")
    endif()
endforeach()
