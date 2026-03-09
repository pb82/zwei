# Zwei — Project Context for Claude

## Overview

A Zelda-style 2D action RPG written in C++ with SDL2. Uses a hand-rolled Entity Component System (ECS), Tiled editor JSON maps, a custom JSON parser, and a C++ scene scripting API. ~10k lines of source code.

**Build system:** CMake (target: Meson)
**Standard:** C++14 (target: C++17)
**Platform:** Linux/macOS (target: Windows + Mac cross-platform)

---

## Architecture

### ECS

- `Entity` holds components in an `unordered_map<type_index, shared_ptr<Component>>`
- `Manager` singleton maintains entities organized by `LayerType` (10 rendering layers)
- Base `Component` has virtual `update()`, `render()`, `key()` methods
- Components access each other and global state via singletons/macros

**Rendering layers (bottom to top):**
`BACKGROUND → FLOOR → WALLS → ROOF → SKY → ITEMS → OBJECTS → FOREGROUND → UI → PARALLAX`

`OBJECTS` layer is Y-sorted for depth illusion.

### Key Components

- `Transform` — position + collision padding
- `Sprite` — texture rendering
- `Animation` — direction-aware frame sequences
- `Collider` — AABB with tags (PLAYER, ENEMY, WALL, PROJECTILE, TRIGGER, ITEM)
- `Controller` — player input
- `Acceleration` — velocity-based movement with configurable acceleration ramp (`acceleration` field, tiles/s²); instant if 0
- `Stats` — HP, strength, dexterity, defense
- `Attack` — weapon/combat system; dispatches to `launchMeleeWeapon` / `launchRangedWeapon`
- `Hitbox` — attached to weapon hitbox entities; carries `power`, `force`, `isRanged`, `origin`, `launchDirection`
- `Inventory` — 10 item slots
- `Ai` — enemy behavior via pluggable `Mind` system

### AI / Mind System

`Mind` base class with virtual `plan()`, `activate()`, `delay()`. Concrete implementations in `src/ecs/minds/`: `Kakta`, `Spider`, `Caterpillar`, `Ally`.

### Scene System

- Base `Scene` with `init()` / `exit()` virtual methods
- Scenes are C++ classes in `src/scn/` that call `Api::*` functions procedurally
- Scene state persisted via `SceneConstants` string keys
- C++ lambdas used as callbacks for triggers/interactions

### Maps

- Tiled editor JSON format (not TMX)
- Custom loader in `src/io/Map.cpp`
- Tileset properties: animation frames, speed, collision padding
- Collision topology extracted from WALLS layer for pathfinding

### JSON Parser

- Fully custom, ~547 lines in `json/`
- Types: STRING, NUMBER, BOOL, ARRAY, OBJECT, NULL
- Used for maps, tilesets, settings/saves

### Global State (Singletons)

- `Manager` — entity management
- `Rt` — runtime context (map, commands queue, scene state)
- `Gfx` — graphics context
- `Draw` — rendering primitives
- `Assets` — texture/font cache
- `St` — persistent settings/save state
- `Player` — audio (SDL_mixer)

Accessed via macros: `RT_Context`, `RT_Player`, `RT_Menu`, `Rt_Map`, `Rt_Commands`.

### Event Bus

`Bus` singleton in `src/Bus.h/cpp`. Decouples cross-cutting reactions from component logic. Subscribe in `main.cpp::initBus()` for game-loop-level concerns; subscribe near the relevant system for domain-specific reactions.

Current events:
- `EventPlayerDied` — published by `Attack::defend()` when player HP hits 0; subscriber pushes `StateGameOver` and plays music
- `EventEnemyDied` (`EnemyDiedEvent` with `x,y`) — published by `Stats::update()` when enemy HP hits 0; subscriber spawns explosion entity
- `EventItemCollected` — published by `Inventory::add()` on successful pickup; subscriber plays sound

To add a new event: add the type to `EventType` enum, optionally subclass `Event` for payload, publish at the source, subscribe in `initBus()` or the relevant system.

### Command Queue

`Rt_Commands` is a deferred queue of `Command` objects executed one per frame. Used for scene transitions (`ScreenTransition`), loading screens, etc. Keep this pattern.

---

## What to Keep

- ECS core (`Entity`, `Component`, `Manager`) — simple and effective, don't replace
- Tiled JSON integration — lightweight and fits the project
- Custom JSON parser — no dependency, handles all needs
- `Mind` AI system — clean abstraction, easy to extend
- Core components: `Stats`, `Animation`, `Collider`, `Attack`, `Inventory`, `Controller`
- Command queue pattern
- Layered rendering pipeline

## What to Strip / Avoid Adding To

- **`src/snd/` (audio)** — not needed yet; removing eliminates SDL2_mixer dependency
- **`imgui/`** — barely used; remove unless actively debugging with it
- **`lua/`** — dead code; remove and start fresh if/when scripting is added
- **Commented-out narrative/speech bubble code** — clean up
- **`BloatComponent`** — test artifact, remove
- **`src/ecs/filters/`** (`Halo`, `Tan`, `Twilight`) — remove until actually used
- **OpenGL context setup** — SDL renderer is used, not GL; drop the `SDL_GL_*` calls

---

## Planned Improvements (Priority Order)

1. **Strip audio + ImGui + dead code** — simplification, low risk
2. **Migrate build to Meson** — enables clean Windows/Mac builds; use WrapDB for SDL2
3. **Bump to C++17** — gets `std::optional`, `std::variant`, `std::filesystem`, `if constexpr`
4. **Replace `Asset` enum with string-keyed registry** — adding assets currently requires editing enum + loader + all references
5. **Add Lua scripting via Sol2** — bind existing `Api::*` functions; write scenes in `.lua` loaded at runtime; enables hot-reload
6. **Simple event bus** ✓ — implemented in `src/Bus.h/cpp`; three events wired up (`EventPlayerDied`, `EventEnemyDied`, `EventItemCollected`)

---

## Known Issues / Tech Debt

- Heavy singleton + macro abuse makes data flow hard to follow
- `CMakeLists.txt` lists every `.cpp` file explicitly
- No entity query system — iteration is manual O(n) per layer
- `Asset` enum requires code changes to add new assets
- Event bus exists but most components still use direct singleton access — migrate gradually
- Scenes (`Forest::init()`) are imperative C++ — will become unmanageable as content grows
- OpenGL context created but SDL renderer used (redundant)

---

## Build

```bash
mkdir build && cd build
cmake ..
make
```

Linux requires SDL2, SDL2_image, SDL2_mixer system packages. macOS uses bundled `.dylib` files.
