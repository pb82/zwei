project('zwei', 'cpp',
  version: '0.1.0',
  default_options: ['cpp_std=c++17'],
)

# System dependencies
sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sdl2_mixer_dep = dependency('SDL2_mixer')
sdl2_ttf_dep = dependency('SDL2_ttf')
lua_dep = dependency('lua', fallback: ['lua', 'lua_dep'])
sol2_dep = dependency('sol2', fallback: ['sol2', 'sol2_dep'])

# Include directories for internal libraries
inc_dirs = include_directories(
  'json',
  'base64',
  'assets',
)

# Internal library sources
json_sources = files(
  'json/parser.cpp',
  'json/printer.cpp',
)

asset_sources = files(
  'assets/Assets.cpp',
  'assets/EMBEDDED/Tiles.cpp',
)

# Game sources
src_sources = files(
  'main.cpp',
  'config.cpp',
  'src/Gfx.cpp',
  'src/Draw.cpp',
  'src/Ctx.cpp',
  'src/Map.cpp',
  'src/Col.cpp',
  'src/Debug.cpp',
  'src/Api.cpp',
  'src/Bus.cpp',
  'src/Font.cpp',
  'src/Lighting.cpp',
  'src/Cc.cpp',
  'src/Mem.cpp',
  'src/St.cpp',
  'src/io/File.cpp',
  'src/io/Out.cpp',
  'src/in/Input.cpp',
  'src/snd/Player.cpp',
  'src/alg/Vector.cpp',
  'src/alg/Color.cpp',
  'src/alg/Force.cpp',
  'src/alg/Path.cpp',
  'src/alg/Rand.cpp',
  'src/alg/Text.cpp',
  'src/ecs/Manager.cpp',
  'src/ecs/Animation.cpp',
  'src/ecs/Sprite.cpp',
  'src/ecs/Tile.cpp',
  'src/ecs/Acceleration.cpp',
  'src/ecs/Controller.cpp',
  'src/ecs/Collider.cpp',
  'src/ecs/Ai.cpp',
  'src/ecs/Stats.cpp',
  'src/ecs/Attack.cpp',
  'src/ecs/SelfDestruct.cpp',
  'src/ecs/Analytics.cpp',
  'src/ecs/Bar.cpp',
  'src/ecs/Hitbox.cpp',
  'src/ecs/Group.cpp',
  'src/ecs/Collectable.cpp',
  'src/ecs/Bloat.cpp',
  'src/ecs/Menu.cpp',
  'src/ecs/Trigger.cpp',
  'src/ecs/Hud.cpp',
  'src/ecs/Interactible.cpp',
  'src/ecs/Timer.cpp',
  'src/ecs/Npc.cpp',
  'src/ecs/char/Character.cpp',
  'src/ecs/inv/Inventory.cpp',
  'src/ecs/ui/Tweak.cpp',
  'src/ecs/items/Item.cpp',
  'src/ecs/items/HealthPotion.cpp',
  'src/ecs/items/StickItem.cpp',
  'src/ecs/items/BowItem.cpp',
  'src/ecs/items/Torch.cpp',
  'src/ecs/items/Heart.cpp',
  'src/ecs/arms/Stick.cpp',
  'src/ecs/arms/Bow.cpp',
  'src/ecs/arms/Stone.cpp',
  'src/ecs/filters/Halo.cpp',
  'src/ecs/filters/Twilight.cpp',
  'src/ecs/filters/Tan.cpp',
  'src/ecs/minds/Mind.cpp',
  'src/ecs/minds/Kakta.cpp',
  'src/ecs/minds/Spider.cpp',
  'src/ecs/minds/Ally.cpp',
  'src/ecs/minds/Caterpillar.cpp',
  'src/scn/Entry.cpp',
  'src/LuaScene.cpp',
)

executable('zwei',
  src_sources,
  json_sources,
  asset_sources,
  include_directories: inc_dirs,
  dependencies: [sdl2_dep, sdl2_image_dep, sdl2_mixer_dep, sdl2_ttf_dep, lua_dep, sol2_dep],
  cpp_args: ['-DPROJECT_SOURCE_DIR="' + meson.project_source_root() + '"'],
)
