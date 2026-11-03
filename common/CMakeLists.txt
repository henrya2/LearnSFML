# Shared code for all games.
# Add public headers under learnsfml/ and implementations under src/; new .cpp
# files are picked up automatically (CONFIGURE_DEPENDS), no edits needed here.
file(GLOB COMMON_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(common STATIC ${COMMON_SOURCES})
add_library(learnsfml::common ALIAS common)

# PUBLIC so games get the include path simply by linking learnsfml::common.
target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(common PUBLIC cxx_std_17)
