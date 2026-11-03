#!/usr/bin/env python3
"""Scaffold a minimal SFML game under games/<name>.

Usage:
    python scripts/new_game.py my_game

The game is picked up automatically by CMake (games/CMakeLists.txt globs with
CONFIGURE_DEPENDS), so a new folder needs no CMake edits -- just reconfigure
or build again. Edit the files in scripts/templates/ to change the skeleton.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
GAMES_DIR = REPO_ROOT / "games"
TEMPLATES_DIR = Path(__file__).resolve().parent / "templates"

# Keep names usable as CMake targets and C++ identifiers.
VALID_NAME = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")

# (template file, destination relative to games/<name>)
FILES = [
    ("CMakeLists.txt.in", "CMakeLists.txt"),
    ("main.cpp.in", "src/main.cpp"),
]


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("Usage: python scripts/new_game.py <game_name>", file=sys.stderr)
        return 2

    name = argv[1]
    if not VALID_NAME.match(name):
        print(
            f"error: '{name}' is not a valid game name "
            "(letters, digits, underscores; must not start with a digit)",
            file=sys.stderr,
        )
        return 2

    game_dir = GAMES_DIR / name
    if game_dir.exists():
        print(f"error: games/{name} already exists", file=sys.stderr)
        return 1

    for template_name, relative_dest in FILES:
        template = (TEMPLATES_DIR / template_name).read_text(encoding="utf-8")
        content = template.replace("@GAME_NAME@", name)
        destination = game_dir / relative_dest
        destination.parent.mkdir(parents=True, exist_ok=True)
        # write_bytes keeps LF endings on every platform.
        destination.write_bytes(content.encode("utf-8"))

    print(f"Created games/{name}/")
    print(f"  games/{name}/CMakeLists.txt")
    print(f"  games/{name}/src/main.cpp")
    print()
    print("Build it with e.g.:")
    print("  cmake -S . -B build -A x64")
    print("  cmake --build build --config Debug")
    print(f"  bin/<platform>/debug/{name}.exe")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
