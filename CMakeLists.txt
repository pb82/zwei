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
        imgui/imgui_impl_opengl2.cpp
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
        src/ecs/Controller.h src/ecs/ui/Tweak.cpp src/ecs/ui/Tweak.h src/ecs/Collider.cpp src/ecs/Collider.h src/Debug.h src/Debug.cpp src/Col.cpp src/Col.h src/alg/Path.cpp src/alg/Path.h)

INCLUDE(FindPkgConfig)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${zwei_SOURCE_DIR}/cmake")
set(LINUX ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
PKG_SEARCH_MODULE(OpenGL gl)

target_include_directories(
        zwei
        PRIVATE ${SDL2_INCLUDE_DIR}
        PRIVATE ${SDL2_IMAGE_INCLUDE_DIR}
        PRIVATE ${OpenGL_INCLUDE_DIRS}
        PRIVATE ${zwei_SOURCE_DIR}/lua
        PRIVATE ${zwei_SOURCE_DIR}/json
        PRIVATE ${zwei_SOURCE_DIR}/imgui
        PRIVATE ${zwei_SOURCE_DIR}/base64
        PRIVATE ${zwei_SOURCE_DIR}/assets
)

target_link_libraries(
        zwei
        PRIVATE ${SDL2_LIBRARY}
        PRIVATE ${SDL2_IMAGE_LIBRARIES}
        PRIVATE ${OpenGL_LIBRARIES}
)


if (LINUX)
    target_link_libraries(
            zwei
            PRIVATE ${zwei_SOURCE_DIR}/lua/linux64/liblua.a
    )
endif (LINUX)