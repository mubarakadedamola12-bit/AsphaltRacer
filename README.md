# AsphaltRacer

An arcade racing game built in Unreal Engine 5.8 (C++). Drive a car around a
procedurally-generated closed-loop track, race two AI opponents, and tune your
ride in the garage.

## Requirements

- Unreal Engine **5.8**
- A C++ toolchain (Xcode on macOS)

## Running it

Open `AsphaltRacer.uproject` in Unreal Editor and press **Play**. The first
launch will compile the C++ module.

```bash
open AsphaltRacer.uproject
```

## Controls

| Action        | Key                              |
|---------------|-----------------------------------|
| Throttle      | W / S (or right/left trigger)    |
| Steer         | A / D (or left stick)            |
| Nitro         | Space                             |
| Drift         | Left Shift                        |
| Garage        | Tab                               |
| Pause menu    | Esc                                |

## What's here

- **Vehicles** (`Source/AsphaltRacer/Vehicles`, `AI`) — a Chaos Vehicles-based
  arcade car with nitro and drift, an AI-driven opponent car, and a shared
  3-car roster (Starter / Speedster / HyperCar) differentiated by tunable
  engine, transmission, and steering stats.
- **Track** (`Source/AsphaltRacer/Tracks`) — `AARTrackManager` builds a closed
  spline loop at runtime, lays out road segments along it, and spawns
  checkpoint gates for lap tracking. No hand-placed level geometry required.
- **Core** (`Source/AsphaltRacer/Core`) — `AARGameMode` procedurally populates
  the level (ground, sky, lighting, track, opponents, player spawn) so the
  project needs only a minimal base map.
- **Systems** (`Source/AsphaltRacer/Systems`) — race/lap manager, garage
  (car selection + upgrades), and progression (XP/credits/unlocks).
- **UI** (`Source/AsphaltRacer/UI`) — HUD, pause menu, and garage screen,
  built natively in C++ (no Widget Blueprints).
- **Content** — Epic's official Offroad Car rig (mesh, wheels, physics
  asset, animation blueprint) and vehicle template track pieces, migrated
  from the engine's Vehicle Advanced template; two synthesized placeholder
  sounds (engine loop, UI click).

## Known limitations

- The whole car roster shares one 3D model, differentiated by physics stats
  rather than appearance.
- Engine and UI audio are simple synthesized placeholders, not licensed sound
  assets.
- There is one track; no track selection or roster.
