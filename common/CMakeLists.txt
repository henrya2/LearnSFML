# Shared code for all games.
# Headers live under include/, implementations under src/. New .cpp files are
# picked up automatically (CONFIGURE_DEPENDS), no edits needed here.
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE COMMON_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

add_library(common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
add_library(learnsfml::common ALIAS common)

# PUBLIC so games get the include path simply by linking learnsfml::common.
target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(common PUBLIC cxx_std_17)
