# Shared code for all games.
# include/ = public headers (exported to games), src/ = private headers.
# New files are picked up automatically (CONFIGURE_DEPENDS), no edits needed.
file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE COMMON_PRIVATE_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE COMMON_PUBLIC_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(common STATIC
    ${COMMON_SOURCES}
    ${COMMON_PRIVATE_HEADERS}
    ${COMMON_PUBLIC_HEADERS})
add_library(learnsfml::common ALIAS common)

# PUBLIC includes are exported to games; PRIVATE ones stay inside the library.
target_include_directories(common
    PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(common PUBLIC cxx_std_17)
