# Super Mario Bros — Group 4

## Overview

This project is a C++20 platform game built with SFML 3.1. It recreates classic Super Mario gameplay with Mario and Luigi, nine stages across three worlds, multiple enemies and bosses, interactive environments, save data, audio, and a leaderboard.

## Features

### Classic Gameplay

- Choose Mario or Luigi.
- Explore three worlds with three levels each.
- Fight Goomba, Koopa, Flying Koopa, Heriss, Petey Piranha, and Bowser.
- Collect coins, mushrooms, and fire flowers.
- Grow into a powered form and shoot fireballs.
- Break bricks and interact with coin and mushroom blocks.

### Game Mechanics

- Physics-based movement, jumping, gravity, and collision handling.
- Sprite-atlas animations for players, enemies, items, and blocks.
- Pipes, trampolines, moving platforms, cannons, bullets, rockets, lava, and win flags.
- Camera tracking across maps larger than the game window.
- Continuous level progression and direct access to all nine stages.

### Additional Systems

- Main menu, character selection, world selection, level selection, pause, death, and win screens.
- Save and continue support.
- Top-ten leaderboard with each player's best score.
- Music, sound effects, and volume settings.
- Responsive game window for different desktop resolutions.

## Technical Details

### Built With

- **C++20** — core programming language.
- **SFML 3.1** — graphics, window, input, and audio.
- **CMake 3.20+** — cross-platform build configuration.

### Architecture

- State-driven application and menu flow.
- AABB physics and map-driven level construction.
- Mediated events for gameplay, score, audio, and level completion.
- Reusable movement and animation components.

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
| Menu navigation | Arrow keys, mouse, `Enter`, or Space |

In the Settings menu, press `M` to toggle music, `S` to toggle sound effects, and Left/Right to change volume.

## Installation and Setup

Download or clone the project, then open a terminal in the folder containing `CMakeLists.txt`. Git and Internet access are needed during the first build if SFML 3.1 is not already installed.

### Visual Studio 2022

1. Install **Visual Studio 2022 Community** or **Visual Studio Build Tools 2022**.
2. In Visual Studio Installer, select **Desktop development with C++** and include MSVC, Windows SDK, and CMake tools.
3. Open **Developer PowerShell for VS 2022** in the project folder and run:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 4
.\build\Release\SuperMario.exe
```

### Visual Studio Code

1. Install Visual Studio Code with the Microsoft **C/C++** and **CMake Tools** extensions.
2. Install CMake 3.20+, MinGW-w64, Ninja, and Git.
3. Add `cmake`, `g++`, `ninja`, and `git` to `PATH`, then restart VS Code.
4. Open a PowerShell terminal in the project folder and verify:

```powershell
cmake --version
g++ --version
ninja --version
git --version
```

5. Configure, build, and run:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
.\build\SuperMario.exe
```

If the compiler or generator changes, remove the old cache before configuring again:

```powershell
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
```

Run the game from the project folder so it can find `assets/`. Save and leaderboard data are created beside the executable.

## Save and Progress

The game can save the player name, score, lives, selected character, current stage, settings, and completed-level progress. The leaderboard stores each player's best result.

## Project Structure

```text
assets/                 Fonts, audio, textures, and level resources
include/                Public headers grouped by subsystem
src/                    Game implementation and main entry point
CMakeLists.txt          Production build configuration
README.md               Project information and setup guide
```

## Credits

Created by CS202 Group 4 as an educational project.

## License Notice

This is a non-commercial fan project. Mario-related characters, names, and concepts belong to Nintendo.
