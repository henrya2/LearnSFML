#pragma once

// Asset paths are resolved relative to the running executable, never relative
// to the process working directory. The build places every game at
//     <repo>/bin/<platform>/<config>/<game>.exe
// while assets live at <repo>/assets, so walking up from the exe finds them.

#include <filesystem>

namespace learnsfml {

// Full path of the currently running executable.
std::filesystem::path executable_path();

// Directory containing the running executable.
std::filesystem::path executable_dir();

// Walk up from the executable until an "assets" directory is found; fall back
// to the known bin/<platform>/<config> depth if none exists yet.
std::filesystem::path assets_dir();

// SFML 3 takes std::filesystem::path, so this feeds loadFromFile directly:
//     texture.loadFromFile(learnsfml::asset_path("player.png"));
std::filesystem::path asset_path(const std::filesystem::path& relative);

} // namespace learnsfml
