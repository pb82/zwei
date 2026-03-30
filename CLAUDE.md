# Zwei — Project Context for Claude

## Overview

A Zelda-style 2D action RPG written in C++ with SDL2. Uses a hand-rolled Entity Component System (ECS), Tiled editor JSON maps, a custom JSON parser, and a C++ scene scripting API. ~10k lines of source code.

**Build system:** Meson (CMakeLists.txt retained for reference but not maintained)
**Standard:** C++17
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

- C++ classes in `src/scn/` with `init()` / `exit()` virtual methods calling `Api::*` procedurally
- Lua scripts via Sol2 (`LuaScene`) with `setup()` / `teardown()` lifecycle, hot-reloadable at runtime (F5)
- Scene state persisted via `SceneConstants` string keys in `Mem`
- `Api::init()` automatically calls `initPlayer()` — scenes don't need to call it explicitly

### Maps

- Tiled editor JSON format (not TMX)
- Loader in `src/Map.cpp` — `Map`, `Layer`, `TileData`
- Layer type derived from Tiled layer **name** (`"floor"`, `"walls"`, etc.) — no custom property needed
- Asset (texture) derived from map's top-level `tilesets[0].source` filename via `assetFromTilesetSource()`
- Tileset animations use native Tiled `animation` array (`[{"duration": 500, "tileid": 35}, ...]`)
- Interact animation stored as plain CSV custom property (`"20,21,22"`)
- Wall tiles use full-tile colliders (no padding) — visual overhangs belong on the ROOF layer
- `loadTilesetIndex()` builds an `unordered_map<int, TileData>` once at load time — O(1) per-tile lookup
- `interactAnimation` on a tile pre-loads state frames onto its `Animation` component; `Api::setDoor()` triggers them via `queueStateFramesForward/Backward()`
- Collision topology (flat `vector<bool>`) extracted from WALLS layer into `RT_Topology` for A* pathfinding — separate from `Collider`-based collision

### JSON Parser

- Fully custom, ~547 lines in `json/`
- Types: STRING, NUMBER, BOOL, ARRAY, OBJECT, NULL
- Used for maps, tilesets, settings/saves

### Global State (Singletons)

- `Manager` — entity management
- `Rt` — runtime context (map, commands queue, scene state)
- `Gfx` — graphics context
- `Draw` — rendering primitives
- `Font` — shared TTF font (m5x7.ttf, scaled by `configZoomFactor`)
- `Assets` — texture cache
- `St` — persistent settings/save state
- `Player` — audio (SDL_mixer)
- `Lighting` — dynamic lighting with darkness overlay

Accessed via macros in `Rt.h`: `RT_Context`, `RT_Camera`, `RT_Player`, `RT_Memory`, `RT_State`, `RT_Spawn`, `RT_Topology`, `Rt_Map`, `Rt_Commands`, `RT_Lighting`.

### Event Bus

`Bus` singleton in `src/Bus.h/cpp`. Decouples cross-cutting reactions from component logic. Subscribe in `main.cpp::initBus()` for game-loop-level concerns; subscribe near the relevant system for domain-specific reactions.

Current events:
- `EventPlayerDied` — published by `Attack::defend()` when player HP hits 0; subscriber publishes `StateChangeRequested(StateGameOver)` and plays music
- `EventEnemyDied` (`EnemyDiedEvent` with `x,y`) — published by `Stats::update()` when enemy HP hits 0; subscriber spawns explosion entity
- `EventItemCollected` — published by `Inventory::add()` on successful pickup; subscriber plays sound
- `EventStateChangeRequested` (`StateChangeRequestedEvent` with `target`) — published by Api/Menu; subscriber calls `RT_State.pushState()`. All state mutations flow through this event.
- `EventQuit` — published by menu quit confirmation; subscriber sets `running = false`

To add a new event: add the type to `EventType` enum, optionally subclass `Event` for payload, publish at the source, subscribe in `initBus()` or the relevant system.

### Lighting System

`Lighting` singleton in `src/Lighting.h/cpp`. Renders a darkness overlay (black rects with per-tile alpha) on top of all world layers, after SKY and before FOREGROUND.

- `addDynamic(Position*, radius, lifetime, tile)` — follows a live Position pointer (player torch)
- `addStatic(x, y, radius, lifetime)` — fixed position (campfire, lantern); lifetime ≤ 0 means permanent
- `getAlphaAt(Position&)` — computes brightness at a world position (max across all sources, `radius / d³` formula)
- All light sources flicker via sine-based radius modulation
- `clear()` on scene teardown removes non-persistent lights; `clearAll()` removes everything
- Serialized/deserialized alongside save games
- Exposed to Lua as `zwei.add_light(x, y, radius, lifetime?)`, `zwei.remove_light(id)`, `zwei.set_enable_lights(bool)`
- Enabled via `HINT_TURN_LIGHTS_OUT` render hint (set by `Api::setEnableLights(false)`)

### Menu System

Custom TTF-based menu in `src/ecs/Menu.cpp`. Replaced ImGui (removed entirely).

- Renders using `Font` singleton (m5x7.ttf via SDL_ttf) and `Draw` primitives
- Semi-transparent blue overlay, vertically centered items, white highlight for selected
- Key-value two-column layout for settings (volume, FPS, window size, key bindings)
- Scrolling viewport for long lists (keyboard/gamepad rebinding)
- Navigation via `GameKeyEvent` (up/down/A/B), no mouse input

### Speech Bubbles

`SpeechBubble` command in `src/Cc.cpp`. TTF-rendered text boxes with typewriter effect.

- Fixed height (3 lines), auto-splits long text via `SpeechBubble::split()` using actual rendered height measurement
- Exposed to Lua as `zwei.speech(text)` (auto-split) and `zwei.speech_single(text, more?)` (single bubble)
- Blinking "more" indicator for multi-bubble sequences
- Press A/B to skip typewriter or advance to next bubble

### Command Queue

`Rt_Commands` is a deferred queue of `Command` objects executed one per frame. Used for scene transitions (`ScreenTransition`), speech bubbles, loading screens, etc. Keep this pattern.

### Display / Resolution

- Fixed virtual resolution: 320x224 (`configVirtualWidth` / `configVirtualHeight`)
- `configZoomFactor = configWindowWidth / configVirtualWidth` — determines tile scaling
- `configWindowWidth` / `configWindowHeight` are mutable (updated for fullscreen)
- Fullscreen via `SDL_WINDOW_FULLSCREEN_DESKTOP` (commented out by default, toggle in `initSdl()`)
- Font size, UI padding, and bubble dimensions all scale by `configZoomFactor`

---

## What to Keep

- ECS core (`Entity`, `Component`, `Manager`) — simple and effective, don't replace
- Tiled JSON integration — lightweight and fits the project
- Custom JSON parser — no dependency, handles all needs
- `Mind` AI system — clean abstraction, easy to extend
- Core components: `Stats`, `Animation`, `Collider`, `Attack`, `Inventory`, `Controller`
- Command queue pattern
- Layered rendering pipeline
- Custom menu system (SDL_ttf + Draw primitives)

## What to Strip / Avoid Adding To

- **`src/snd/` (audio)** — not needed yet; removing eliminates SDL2_mixer dependency
- **`imgui/` directory** — no longer compiled or referenced; can be deleted from disk
- **Commented-out narrative/speech bubble code** — clean up
- **`BloatComponent`** — test artifact, remove
- **`src/ecs/filters/`** (`Halo`, `Tan`, `Twilight`) — remove until actually used
- **OpenGL context setup** — SDL renderer is used, not GL; drop the `SDL_GL_*` calls
- **`assets/EMBEDDED/Font.h/.cpp`** — embedded ImGui font, no longer used; can be deleted

---

## Planned Improvements (Priority Order)

1. **Strip audio + dead code** — simplification, low risk
2. **Migrate build to Meson** ✓ — `meson.build` at project root; dependencies via pkg-config; `meson setup builddir && meson compile -C builddir`
3. **Bump to C++17** ✓ — `default_options: ['cpp_std=c++17']` in meson.build
4. **Replace `Asset` enum with string-keyed registry** — adding assets currently requires editing enum + loader + all references; `assetFromTilesetSource()` in Map.cpp is the last place that maps filenames to the enum
5. **Add Lua scripting via Sol2** ✓ — `LuaScene` class; Lua + Sol2 as Meson wraps; `Api` exposed as `zwei` table in Lua; scenes in `scenes/*.lua`; F5 hot-reload
6. **Simple event bus** ✓ — `src/Bus.h/cpp`; five events wired up; state mutations flow through `EventStateChangeRequested`
7. **Map loader cleanup** ✓ — `loadTilesetIndex()`; native Tiled animations; wall tiles use full-tile colliders (no tile padding)
8. **Tile padding removed from walls** ✓ — wall tiles always use full-tile colliders; visual overhangs belong on ROOF layer; padding retained for entity/projectile colliders only
9. **Lighting system** ✓ — `src/Lighting.h/cpp`; dynamic + static lights; darkness overlay; Lua bindings; serialize/deserialize
10. **Replace ImGui with custom menu** ✓ — SDL_ttf + Draw primitives; scrolling; `imgui/` directory can be deleted
11. **Fixed virtual resolution** ✓ — 320x224 SNES-style viewport; fullscreen desktop support

---

## Known Issues / Tech Debt

- Heavy singleton + macro abuse makes data flow hard to follow — migrating gradually via event bus
- No entity query system — iteration is manual O(n) per layer
- `Asset` enum requires code changes to add new assets
- Scenes (`Forest::init()`) are imperative C++ — being replaced by Lua scripts (`scenes/*.lua`)
- OpenGL context created but SDL renderer used (redundant)
- `imgui/` directory and `assets/EMBEDDED/Font.*` still on disk but no longer compiled — delete when convenient

---

## Build

```bash
meson setup builddir
meson compile -C builddir
./builddir/zwei
```

Linux requires SDL2, SDL2_image, SDL2_mixer, SDL2_ttf system packages (`libsdl2-dev`, `libsdl2-image-dev`, `libsdl2-mixer-dev`, `libsdl2-ttf-dev`).

### Tooling

- `tools/create-asset` — Python script to scaffold new maps and tilesets for the Tiled editor
