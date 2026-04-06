# Lua Scripting API

Zwei scenes can be written in Lua. Each scene is a `.lua` file in this directory, registered in `Ctx.cpp` as a `LuaScene`. The Lua runtime exposes the `zwei` table with functions to control the game.

Scenes are hot-reloadable at runtime with F5.

## Scene Hooks

A scene script may define two global functions:

### `setup()`

Called when the scene is entered. Use this to load the map, place the player, spawn enemies, set up triggers, etc. The player entity is already initialized before `setup()` is called.

```lua
function setup()
    zwei.set_player_position(2, 30)
    zwei.load_map("Forest.json")
    zwei.set_game_state()
end
```

### `teardown()`

Called when the scene is exited. Use this to save player position, unload the map, and clean up any scene-specific state.

```lua
function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
```

## Standard Libraries

The following Lua standard libraries are available: `base`, `string`, `math`, `table`.

---

## API Reference

All functions are accessed through the `zwei` table.

### `zwei.load_map(file)`

Loads a Tiled JSON map from the `assets/NEW/MAPS` directory. Sets up the camera bounds and collision topology.

```lua
zwei.load_map("Forest.json")
```

### `zwei.unload_map()`

Unloads the current map, clears collision topology, resets the camera, and removes all entities.

```lua
zwei.unload_map()
```

### `zwei.set_player_position(x, y)`

Sets the player's position in tile coordinates. If a position was previously pushed onto the spawn stack, that position is used instead and the arguments are ignored.

```lua
zwei.set_player_position(2, 30)
```

### `zwei.push_player_position()`

Saves the player's current position onto an internal stack. Typically called in `teardown()` so the player can be restored to the same position when re-entering the scene.

```lua
function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
```

### `zwei.pop_player_position()`

Pops and returns the last saved player position from the stack. Returns two values `x, y` on success, or nothing if the stack is empty.

```lua
local x, y = zwei.pop_player_position()
if not x then
    x, y = 2, 30  -- default spawn
end
zwei.set_player_position(x, y)
```

### `zwei.set_player_speed(speed)`

Sets the player's maximum movement speed in tiles per second.

```lua
zwei.set_player_speed(5)
```

### `zwei.set_player_stats(hp, str, dex, def)`

Sets the player's base stats: hit points, strength, dexterity, and defense.

```lua
zwei.set_player_stats(20, 1, 1, 1)
```

### `zwei.set_game_state()`

Transitions the game into the playing state. Call this after loading the map and placing the player.

```lua
zwei.load_map("Forest.json")
zwei.set_player_position(2, 30)
zwei.set_game_state()
```

### `zwei.set_enable_lights(enabled)`

Enables or disables the darkness overlay. When `false`, a darkness layer is rendered over the world and only light sources illuminate the scene.

```lua
zwei.set_enable_lights(false)  -- turn on darkness
zwei.add_light(10, 15, 4)     -- add a light so the player can see
```

### `zwei.add_light(x, y, radius, lifetime?)`

Adds a static light source at the given tile coordinates. Returns an integer light ID that can be used with `remove_light`. The optional `lifetime` is in seconds; if omitted or 0, the light is permanent.

```lua
-- permanent campfire light
local id = zwei.add_light(10, 15, 4)

-- temporary flash that lasts 2 seconds
zwei.add_light(5, 5, 6, 2.0)
```

### `zwei.remove_light(id)`

Removes a light source by its ID (as returned by `add_light`).

```lua
local id = zwei.add_light(10, 15, 4)
-- later...
zwei.remove_light(id)
```

### `zwei.speech(text)`

Displays a speech bubble with the given text. Long text is automatically split across multiple bubbles (3 lines each). The player presses A/B to advance through them.

```lua
zwei.speech("Hello, adventurer! Welcome to the forest.")
```

### `zwei.speech_single(text, more?)`

Displays a single speech bubble. If `more` is `true`, a blinking indicator is shown to signal that another bubble follows. Use this for manual control over multi-bubble sequences.

```lua
zwei.speech_single("First line of dialogue.", true)
zwei.speech_single("Second and final line.", false)
```

### `zwei.set_trigger(x, y, on_enter, on_exit, proximity?)`

Creates a trigger zone at tile position `(x, y)`. The `on_enter` callback fires when the player enters the area; `on_exit` fires when the player leaves. Both callbacks receive the approach angle as their argument. The optional `proximity` value expands the trigger's collider in all directions.

```lua
zwei.set_trigger(10, 20,
    function(angle)
        zwei.speech("You entered the zone!")
    end,
    function(angle)
        -- player left
    end,
    1.0  -- expand collider by 1 tile
)
```

---

## Full Example

```lua
function setup()
    local x, y = zwei.pop_player_position()
    if not x then x, y = 2, 30 end

    zwei.set_player_position(x, y)
    zwei.load_map("Forest.json")
    zwei.set_game_state()

    zwei.set_trigger(15, 25,
        function(angle)
            zwei.speech("Watch out, enemies ahead!")
        end,
        function(angle) end
    )
end

function teardown()
    zwei.push_player_position()
    zwei.unload_map()
end
```
