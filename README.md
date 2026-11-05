# LearnSFML

A small C++ workspace for learning and experimenting with [SFML 3](https://www.sfml-dev.org/).
Rather than one big program, the repository is a collection of **independent mini-games**,
each its own executable. The build system is deliberately lightweight: adding a game is a
single command and requires no edits to any CMake file.

Under the hood:

- **SFML 3.1.0** is vendored as a git submodule at `thirdparty/SFML` (pinned to tag `3.1.0`).
- **C++17** everywhere (SFML 3 is built on C++17).
- **Static linking by default**, so each built game is one self-contained `.exe` with no DLLs
  to ship.
- A shared static library, `learnsfml::common`, provides helpers (currently asset-path
  resolution) available to every game.
- Each game links `SFML::Graphics` and `learnsfml::common` automatically.

---

## Requirements

| Tool | Version / notes |
| --- | --- |
| CMake | **3.28 or newer** (required by SFML 3.1.0) |
| C++ compiler | Any C++17 compiler (MSVC, GCC, Clang, AppleClang) |
| Git | For cloning the SFML submodule |
| Python 3 | Only needed for the `new_game.py` scaffolding script |

The project is platform-independent. Binaries are written under a platform-specific
folder (`win64`, `macos64`, `linux64`, ...). The checked-in `build/` tree here was generated
with the Visual Studio generator on Windows, but Ninja / Makefiles work too.

---

## Getting started

Clone **with submodules**, otherwise `thirdparty/SFML` will be empty:

```bash
git clone --recurse-submodules <repo-url> LearnSFML
cd LearnSFML
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

### Configure and build

On Windows with Visual Studio (the generator already in use in this repo):

```bash
cmake -S . -B build -A x64
cmake --build build --config Debug
```

On Linux/macOS with a single-config generator (pick a build type):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run a game from its output folder, for example:

```bash
bin/win64/debug/hello_sfml.exe
```

> You never need to re-run the configure step manually to pick up new games or source files:
> the CMake globs use `CONFIGURE_DEPENDS`, so the next build re-scans automatically.

---

## Project layout

```
LearnSFML/
├── CMakeLists.txt              # Top-level: C++17, output dirs, SFML, subdirs
├── cmake/
│   └── Game.cmake              # Defines learnsfml_add_game(<name>)
├── common/                     # Shared static library -> learnsfml::common
│   ├── CMakeLists.txt
│   ├── include/learnsfml/      # PUBLIC headers (exported to every game)
│   │   └── paths.hpp
│   └── src/                    # Private headers + .cpp implementation
│       └── paths.cpp
├── games/                      # One folder per game; auto-discovered
│   ├── CMakeLists.txt          # Globs subfolders that contain a CMakeLists.txt
│   ├── hello_sfml/             # Example: a bouncing-free green circle
│   └── show_image/             # Example: displays assets/logo.png
├── assets/                     # Shared assets at the repo root (e.g. logo.png)
├── scripts/
│   ├── new_game.py             # Scaffolds a new game
│   └── templates/              # Skeleton files used by new_game.py
├── bin/                        # Build output: bin/<platform>/<config>/
│   └── win64/{debug,release,relwithdebinfo}/
├── thirdparty/SFML/            # SFML git submodule (do not edit)
└── build/                      # CMake build tree (git-ignored)
```

### A single game's structure

```
games/<name>/
├── CMakeLists.txt      # Just: learnsfml_add_game(<name>)
├── include/            # PUBLIC headers (*.h / *.hpp); exported via target_include_directories
└── src/                # *.cpp sources and PRIVATE headers (*.h / *.hpp)
```

`learnsfml_add_game()` (in `cmake/Game.cmake`) does all the work:

- Globs `src/**/*.cpp` as sources and `src/**/*.h(pp)` + `include/**/*.h(pp)` as headers.
- Adds `include/` as a **PUBLIC** include path and `src/` as a **PRIVATE** one.
- Links `SFML::Graphics` and `learnsfml::common`.
- Places the executable in `bin/<platform>/<config>/`.
- In shared builds, copies SFML's DLLs next to the `.exe` so it starts as-is.

The same public/private header split is used by `common/`. Drop shared helpers into
`common/include/learnsfml/` and `#include <learnsfml/...>` from any game — no CMake edits
needed, new files are picked up automatically.

---

## Creating a new game

Use the scaffolding script:

```bash
python scripts/new_game.py my_game
```

This creates `games/my_game/` with a ready-to-build skeleton:

```
games/my_game/
├── CMakeLists.txt      # learnsfml_add_game(my_game)
├── include/.gitkeep    # place public headers here
└── src/main.cpp        # minimal SFML window + event loop
```

Notes:

- The name must be a valid CMake target / C++ identifier: letters, digits, underscores,
  and it cannot start with a digit (`my_game` ✓, `2d-game` ✗).
- `games/CMakeLists.txt` auto-discovers any subfolder containing a `CMakeLists.txt`, so
  **no central registration is required**. Just reconfigure or rebuild.
- Customize the generated skeleton by editing the files in `scripts/templates/`
  (`CMakeLists.txt.in`, `main.cpp.in`, `gitkeep.in`). The token `@GAME_NAME@` is replaced
  with the name you pass in.

Then build and run:

```bash
cmake --build build --config Debug
bin/win64/debug/my_game.exe
```

---

## Assets and how paths work

All shared assets live in the top-level **`assets/`** directory (currently `logo.png`).
Because executables are written to `bin/<platform>/<config>/` — several levels below the
repo root — you should **not** load files relative to the process working directory.
The `common` library solves this with `learnsfml::asset_path()` (declared in
`common/include/learnsfml/paths.hpp`):

```cpp
#include <learnsfml/paths.hpp>

sf::Texture texture;
if (!texture.loadFromFile(learnsfml::asset_path("logo.png")))
    return EXIT_FAILURE;
```

`assets_dir()` walks up from the running executable until it finds an `assets/` folder
(and falls back to the known `bin/<platform>/<config>` depth if none exists yet), so paths
work regardless of where you launch the game from. `asset_path()` returns a
`std::filesystem::path`, which SFML 3's `loadFromFile` accepts directly. Other helpers in
the same header: `executable_path()` and `executable_dir()`.

Drop new images, fonts, or sounds into `assets/` and reference them by name. See
`games/show_image/src/main.cpp` for a complete example.

---

## Build options

All are standard CMake cache variables; pass them at configure time with `-D`:

| Option | Default | Purpose |
| --- | --- | --- |
| `BUILD_SHARED_LIBS` | `OFF` | Build SFML as shared libraries (DLLs are copied next to each `.exe`). |
| `SFML_BUILD_AUDIO` | `OFF` | Enable the SFML audio module (pulls in FLAC/Vorbis/...). |
| `SFML_BUILD_NETWORK` | `OFF` | Enable the SFML network module (pulls in MbedTLS/...). |
| `CMAKE_BUILD_TYPE` | `Debug` | Build type for single-config generators (Debug/Release/RelWithDebInfo). |

Audio and network are off by default to keep a fresh configure lean. Enable them only when
a game needs them, e.g.:

```bash
cmake -S . -B build -DSFML_BUILD_AUDIO=ON
```

SFML's own examples, tests, and docs are disabled (`SFML_CONFIGURE_EXTRAS OFF`) since it
is used here purely as a subproject.

---

## Example games

| Game | What it shows |
| --- | --- |
| `hello_sfml` | The smallest possible SFML 3 window: creates a render window, draws a green circle, and handles the close event. |
| `show_image` | Loads `assets/logo.png` through `learnsfml::asset_path()` and draws it as a sprite. |

Both were generated from `scripts/new_game.py`, so they mirror exactly what a new game
starts with.

---

## Other notes

- **Output layout** — binaries go to `bin/<platform>/<config>/<game>`, where `<platform>`
  is `win64` / `macos64` / `linux64` (derived from pointer size and OS) and `<config>` is
  the lowercased build configuration (`debug`, `release`, `relwithdebinfo`). A generator
  expression prevents multi-config generators (Visual Studio) from appending an extra
  `Debug/`, `Release/` folder.
- **Version control** — `build/`, `/bin/**` binaries, editor/OS noise are git-ignored;
  only the `.gitkeep` placeholders in `bin/` are tracked to document the folder layout.
  `thirdparty/SFML` is a submodule — commit the submodule pointer, not its contents.
- **Adding shared code** — put reusable helpers in `common/`; they are compiled once into
  a static library and linked into every game.
- **SFML API version** — this targets **SFML 3** (e.g. `sf::RenderWindow(sf::VideoMode({w,h}), "title")`,
  `window.pollEvent()` returning an optional, `event->is<sf::Event::Closed>()`). SFML 2
  tutorials/code will not compile unchanged.
