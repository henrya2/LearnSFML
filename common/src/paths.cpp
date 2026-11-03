#include <learnsfml/paths.hpp>

#include <stdexcept>
#include <string>
#include <system_error>

#if defined(_WIN32)
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#   include <windows.h>
#elif defined(__APPLE__)
#   include <cstdint>
#   include <mach-o/dyld.h>
#else
#   include <limits.h>   // PATH_MAX
#   include <unistd.h>   // readlink
#endif

namespace learnsfml {

std::filesystem::path executable_path()
{
#if defined(_WIN32)
    std::wstring buffer(MAX_PATH, L'\0');
    for (;;)
    {
        const DWORD length = ::GetModuleFileNameW(
            nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length == 0)
            throw std::runtime_error("learnsfml: GetModuleFileNameW failed");
        if (length < buffer.size())
        {
            buffer.resize(length);
            break;
        }
        buffer.resize(buffer.size() * 2);
    }
    return std::filesystem::path(buffer);
#elif defined(__APPLE__)
    std::uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::string buffer(size, '\0');
    if (_NSGetExecutablePath(buffer.data(), &size) != 0)
        throw std::runtime_error("learnsfml: _NSGetExecutablePath failed");
    return std::filesystem::canonical(std::filesystem::path(buffer));
#else
    std::string buffer(PATH_MAX, '\0');
    const ssize_t length = ::readlink("/proc/self/exe", buffer.data(), buffer.size());
    if (length <= 0)
        throw std::runtime_error("learnsfml: readlink(/proc/self/exe) failed");
    buffer.resize(static_cast<std::size_t>(length));
    return std::filesystem::canonical(std::filesystem::path(buffer));
#endif
}

std::filesystem::path executable_dir()
{
    return executable_path().parent_path();
}

std::filesystem::path assets_dir()
{
    std::filesystem::path dir = executable_dir();
    for (int depth = 0; depth < 8; ++depth)
    {
        std::error_code ec;
        if (std::filesystem::is_directory(dir / "assets", ec))
            return dir / "assets";

        const std::filesystem::path parent = dir.parent_path();
        if (parent == dir)
            break;
        dir = parent;
    }
    return executable_dir() / ".." / ".." / ".." / "assets";
}

std::filesystem::path asset_path(const std::filesystem::path& relative)
{
    return assets_dir() / relative;
}

} // namespace learnsfml
