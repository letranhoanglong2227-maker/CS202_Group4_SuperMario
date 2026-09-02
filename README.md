# Super Mario — Group 4

## Overview

This project is a C++20/SFML 3.1 platform game inspired by classic Super Mario gameplay. It provides a complete menu-to-game flow, Mario and Luigi, nine stages across three worlds, enemies, bosses, items, hazards, moving platforms, save data, audio, HUD, and a top-ten leaderboard.

The implementation uses deterministic enemy behavior, a state-driven application flow, and `std::unique_ptr`-based runtime ownership.

## Gameplay Features

- Choose Mario or Luigi.
- Explore three worlds with three levels each.
- Enter any of the nine maps directly from the world/level menus.
- Fight Goomba, Koopa, Flying Koopa, Heriss, Petey Piranha, and Bowser.
- Use blocks, pipes, trampolines, moving platforms, cannons, lava, rockets, bullets, and win flags.
- Collect coins and mushrooms, grow into a powered form, and shoot fireballs.
- Continue through consecutive stages after completing a level.
- Save and continue a session from the main menu.
- Submit scores from any map to a top-ten leaderboard. Each player name keeps its best score and the stage where that score was achieved.
- Configure music, sound effects, and volume.

## Controls

| Action | Keyboard / mouse |
|---|---|
| Move | `A` / `D` or Left / Right arrows |
| Jump | `W`, Up arrow, `J`, Numpad `1`, or Space |
| Run | Left `Ctrl` |
| Crouch while Big/Fire | `S` or Down arrow |
| Shoot horizontally while powered | `K` or Numpad `0` |
| Aim and shoot while powered | Left mouse button |
| Pause / return | `Esc` |
| Menu navigation | Arrow keys, mouse, `Enter` / Space where shown |

Settings shortcuts:

- `M`: toggle music.
- `S`: toggle sound effects.
- Left / Right arrows: decrease or increase volume.

## Technical Design

- **Language:** C++20
- **Framework:** SFML 3.1
- **Build system:** CMake 3.20 or newer
- **Ownership:** runtime entities are owned with RAII and `std::unique_ptr`; borrowed pointers are non-owning views only
- **State flow:** main menu, name entry, character/world/level selection, gameplay, pause, death, win, settings, and leaderboard states
- **Runtime:** map-driven level construction, AABB physics, bounded camera, event mediator, deferred entity adoption/removal, and deterministic collision outcomes
- **Presentation:** sprite-atlas animation, world backgrounds, screen-space HUD, music, and sound effects
- **Persistence:** versioned save data and one-best-entry-per-player leaderboard files

## Design Patterns

The project applies five design patterns required by its architecture:

1. **State:** `State` and `StateStack` manage menus, gameplay, pause, death, and completion screens without a monolithic application loop.
2. **Mediator:** `GameEventMediator` routes typed gameplay, score, life, completion, and audio events between otherwise independent systems.
3. **Singleton:** `AudioSystem::instance()` provides one process-wide audio resource service through a function-local static instance.
4. **Simple Factory:** `EntityFactory` and the level factory create concrete players, enemies, items, and stages from validated identifiers.
5. **Component:** `LivingEntity` composes reusable `MovementComponent` and `AnimationComponent` behaviors.

## Project Layout

```text
assets/                 Fonts, audio, textures, and level resources
include/                Public headers grouped by subsystem
src/                    Production implementation and main entry point
CMakeLists.txt          Production build configuration
```

## Build and Run

### Requirements

- CMake 3.20 or newer.
- A C++20 toolchain. On Windows, install either:
  - Visual Studio 2022 Build Tools with the **Desktop development with C++** workload; or
  - MinGW-w64 GCC together with Ninja, with both `g++` and `ninja` available in `PATH`.
- SFML 3.1 installed, or Git and Internet access during the first configure so CMake can fetch SFML automatically.

Installing CMake alone is not enough because CMake coordinates a compiler and a build tool; it does not replace them.

### Windows with MinGW and Ninja

Open PowerShell in the project root and verify the tools:

```powershell
cmake --version
g++ --version
ninja --version
```

Configure, build, and run:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
.\build\SuperMario.exe
```

### Windows with Visual Studio Build Tools

Open **Developer PowerShell for VS 2022** in the project root, then run:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 4
.\build\Release\SuperMario.exe
```

### Generator or compiler errors

If CMake reports that `nmake`, `ninja`, `cl`, or a C++ compiler cannot be found, install one of the complete toolchains above and use its matching generator explicitly.

A build directory remembers its generator. Delete that directory before changing from NMake, Visual Studio, or Ninja:

```powershell
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
```

Then run the configure command for the selected toolchain again.

The normal development build finds `assets/` in the project root. For a standalone package, place the complete `assets/` directory beside `SuperMario.exe`. Save and leaderboard files are created beside the executable.

## Current Verification

- Production target builds successfully.
- Human playtesting has covered the main menu-to-game route and representative level, enemy, item, moving-platform, boss, save, and leaderboard behavior.

## Educational Use

This project was created by CS202 Group 4 for educational purposes. Mario-related characters, names, and concepts belong to Nintendo. This is a non-commercial fan project.
