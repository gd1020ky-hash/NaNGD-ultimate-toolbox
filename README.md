# Ultimate Toolbox for Geometry Dash

An open-source Geode mod featuring an all-in-one utility suite for Geometry Dash 2.2081+ on **Geode SDK v5.11.2**.

## Features
- **Gameplay:** Noclip, Hitbox visualizer, Practice Mode music sync, Instant Respawn, Confirm exit on >75%.
- **Speedhack & Audio Sync:** Adjusts time scale from 0.1x to 3.0x while syncing FMOD music pitch.
- **Diagnostics:** High-accuracy microsecond frame profiling, jitter analysis, 1% low FPS, and physics tick frame window counter.
- **HUD & Visuals:** Live FPS, rolling CPS counter, 2-decimal accurate percentage, and continuous HSV Rainbow Trail.
- **Audio:** NanGD metallic service desk bell click sound powered by FMOD.
- **Controls & Safety:** Safe Mode to protect leaderboards; press `Tab` or `C` in-game to toggle the Toolbox GUI.

## Quick Start
1. Install [CMake 3.21+](https://cmake.org/) and the [Geode CLI](https://docs.geode-sdk.org/).
2. Run `python generate_bell.py` (or place your own `bell.wav` in `resources/`).
3. Compile the mod:
   ```bash
   geode build
   ```
