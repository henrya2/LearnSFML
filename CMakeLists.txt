cmake_minimum_required(VERSION 3.28)   # SFML 3.1.0 requires at least 3.28

project(LearnSFML LANGUAGES CXX)

# ---------------------------------------------------------------------------
# C++17 -- SFML 3 is built on C++17, so every game uses it too.
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Output layout: bin/<platform>/<config>, e.g. bin/win64/debug/my_game.exe
# ---------------------------------------------------------------------------
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_learnsfml_bits 64)
else()
    set(_learnsfml_bits 32)
endif()

if(WIN32)
    set(LEARNSFML_PLATFORM "win${_learnsfml_bits}")
elseif(APPLE)
    set(LEARNSFML_PLATFORM "macos${_learnsfml_bits}")
elseif(UNIX)
    set(LEARNSFML_PLATFORM "linux${_learnsfml_bits}")
else()
    set(LEARNSFML_PLATFORM "unknown${_learnsfml_bits}")
endif()

# A generator expression keeps multi-config generators (Visual Studio) from
# appending an extra /Debug, /Release, ... on top of our own config folder.
set(LEARNSFML_BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${LEARNSFML_PLATFORM}/$<LOWER_CASE:$<CONFIG>>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${LEARNSFML_BIN_DIR}")

# Single-config generators have no <CONFIG> unless a build type is chosen.
get_property(_learnsfml_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT _learnsfml_multi_config AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# SFML -- git submodule at thirdparty/SFML, pinned to tag 3.1.0
# ---------------------------------------------------------------------------
# Static by default so a built game is one self-contained .exe (no DLLs to ship).
if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build SFML as static libraries")
endif()

# Audio/Network pull in extra vendored dependencies (FLAC/Vorbis/MbedTLS/...).
# Off by default to keep a fresh configure lean; enable when a game needs them:
#   -DSFML_BUILD_AUDIO=ON  /  -DSFML_BUILD_NETWORK=ON
if(NOT DEFINED SFML_BUILD_AUDIO)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "Build the SFML audio module")
endif()
if(NOT DEFINED SFML_BUILD_NETWORK)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "Build the SFML network module")
endif()

# Skip SFML's own examples / tests / docs when it is used as a subproject.
set(SFML_CONFIGURE_EXTRAS OFF)

add_subdirectory(thirdparty/SFML)

# ---------------------------------------------------------------------------
# Games
# ---------------------------------------------------------------------------
include(cmake/Game.cmake)   # defines learnsfml_add_game(<name>)
add_subdirectory(common)     # shared static library (learnsfml::common)
add_subdirectory(games)      # auto-discovers every games/<name> folder
