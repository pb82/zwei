cmake_minimum_required(VERSION 3.15)
project(zwei)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        zwei
        main.cpp
        json/
        json/printer.cpp
        json/parser.cpp
        imgui/imgui.cpp
        imgui/imgui_impl_sdl.cpp
        imgui/imgui_sdl.cpp
        imgui/imgui_widgets.cpp
        imgui/imgui_draw.cpp
        assets/EMBEDDED/EmbeddedAsset.h
        assets/EMBEDDED/Font.h assets/EMBEDDED/Font.cpp
        assets/EMBEDDED/Tiles.h assets/EMBEDDED/Tiles.cpp
        config.h
        src/Gfx.h src/Gfx.cpp
        src/Rt.h src/Cam.h src/ecs/Component.h src/ecs/Transform.h src/ecs/Entity.h
        src/alg/Position.h
        assets/Assets.cpp assets/ASSETS/Assets.h src/ecs/Tile.h src/ecs/Tile.cpp
        src/Draw.cpp src/Draw.h config.cpp src/io/File.cpp src/io/File.h src/Map.cpp src/Map.h
        src/ecs/Manager.h src/ecs/Manager.cpp src/ecs/Animation.cpp src/ecs/Animation.h
        src/ecs/Sprite.cpp src/ecs/Sprite.h src/alg/Vector.h src/alg/Vector.cpp src/alg/Direction.h
        src/alg/Color.h src/alg/Color.cpp src/ecs/Acceleration.h src/ecs/Acceleration.cpp
        src/alg/Force.cpp src/alg/Force.h src/in/Input.cpp src/in/Input.h src/ecs/Controller.cpp
        src/ecs/Controller.h src/ecs/ui/Tweak.cpp src/ecs/ui/Tweak.h src/ecs/Collider.cpp src/ecs/Collider.h src/Debug.h src/Debug.cpp src/Col.cpp src/Col.h src/alg/Path.cpp src/alg/Path.h src/ecs/Ai.cpp src/ecs/Ai.h src/ecs/minds/Mind.h src/ecs/minds/Kakta.cpp src/ecs/minds/Kakta.h src/Ctx.cpp src/Ctx.h src/ecs/arms/Weapon.h src/ecs/Stats.cpp src/ecs/Stats.h src/ecs/Attack.cpp src/ecs/Attack.h src/ecs/SelfDestruct.cpp src/ecs/SelfDestruct.h src/alg/Rand.h src/ecs/filters/Filter.h src/ecs/filters/Halo.cpp src/ecs/filters/Halo.h src/ecs/arms/Stick.cpp src/ecs/arms/Stick.h src/ecs/filters/Twilight.cpp src/ecs/filters/Twilight.h src/ecs/Analytics.cpp src/ecs/Analytics.h src/alg/Padding.h src/ecs/Bar.cpp src/ecs/Bar.h src/ecs/Projectile.cpp src/ecs/Projectile.h src/ecs/minds/Spider.cpp src/ecs/minds/Spider.h src/ecs/char/Character.cpp src/ecs/char/Character.h src/ecs/Group.cpp src/ecs/Group.h src/alg/Rand.cpp src/alg/Text.cpp src/alg/Text.h src/ecs/items/Item.h src/ecs/Collectable.cpp src/ecs/Collectable.h src/ecs/items/HealthPotion.cpp src/ecs/items/HealthPotion.h src/ecs/inv/Inventory.cpp src/ecs/inv/Inventory.h src/ecs/Bloat.cpp src/ecs/Bloat.h src/ecs/items/Item.cpp src/ecs/items/StickItem.cpp src/ecs/items/StickItem.h src/snd/Player.cpp src/snd/Player.h src/ecs/Menu.cpp src/ecs/Menu.h src/St.cpp src/St.h src/io/Out.cpp src/io/Out.h src/ecs/Trigger.cpp src/ecs/Trigger.h src/ecs/Hud.cpp src/ecs/Hud.h src/ecs/arms/Bow.cpp src/ecs/arms/Bow.h src/ecs/items/BowItem.cpp src/ecs/items/BowItem.h src/ecs/arms/Stone.cpp src/ecs/arms/Stone.h imgui/IMGUI/imgui_sdl.h imgui/imgui_sdl.cpp src/Cc.cpp src/Cc.h src/Api.cpp src/Api.h src/ecs/Interactible.cpp src/ecs/Interactible.h src/Scene.h src/Mem.cpp src/Mem.h src/ecs/Timer.cpp src/ecs/Timer.h src/ecs/items/Torch.cpp src/ecs/items/Torch.h src/ecs/inv/Modifier.h src/ecs/inv/TorchModifier.cpp src/ecs/inv/TorchModifier.h src/ecs/Id.h src/ecs/minds/Ally.cpp src/ecs/minds/Ally.h src/ecs/Hostile.h src/ecs/minds/Mind.cpp src/ecs/filters/Tan.h src/ecs/filters/Tan.cpp src/ecs/Friend.h src/ecs/items/Heart.cpp src/ecs/items/Heart.h src/ecs/Npc.cpp src/ecs/Npc.h src/scn/Ship_LowerDeck.cpp src/scn/Ship_LowerDeck.h src/ecs/minds/Caterpillar.cpp src/ecs/minds/Caterpillar.h src/scn/EntryPoint.cpp src/scn/EntryPoint.h src/ecs/LightSource.cpp src/ecs/LightSource.h)

INCLUDE(FindPkgConfig)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${zwei_SOURCE_DIR}/cmake")

if (APPLE)
    target_include_directories(
            zwei
            PRIVATE ${zwei_SOURCE_DIR}/json
            PRIVATE ${zwei_SOURCE_DIR}/imgui
            PRIVATE ${zwei_SOURCE_DIR}/base64
            PRIVATE ${zwei_SOURCE_DIR}/assets
            PRIVATE ${zwei_SOURCE_DIR}/sdl/darwin/SDL
            PRIVATE ${zwei_SOURCE_DIR}/sdl/darwin/IMAGE
            PRIVATE ${zwei_SOURCE_DIR}/sdl/darwin/MIXER
    )

    target_link_libraries(
            zwei
            ${zwei_SOURCE_DIR}/sdl/darwin/libSDL2.dylib
            ${zwei_SOURCE_DIR}/sdl/darwin/libSDL2_image.dylib
            ${zwei_SOURCE_DIR}/sdl/darwin/libSDL2_mixer.dylib
    )
endif ()

if (UNIX AND NOT APPLE)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)

    target_include_directories(
            zwei
            PRIVATE ${zwei_SOURCE_DIR}/json
            PRIVATE ${zwei_SOURCE_DIR}/imgui
            PRIVATE ${zwei_SOURCE_DIR}/base64
            PRIVATE ${zwei_SOURCE_DIR}/assets
            PRIVATE ${zwei_SOURCE_DIR}/sdl/linux/SDL
            PRIVATE ${zwei_SOURCE_DIR}/sdl/linux/IMAGE
            PRIVATE ${zwei_SOURCE_DIR}/sdl/linux/MIXER
    )

    target_link_libraries(
            zwei
            PRIVATE ${SDL2_LIBRARY}
            PRIVATE ${SDL2_IMAGE_LIBRARIES}
            PRIVATE ${SDL2_MIXER_LIBRARIES}
    )

    add_custom_command(TARGET zwei
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:zwei> ${zwei_SOURCE_DIR}/bin/linux)
endif ()

