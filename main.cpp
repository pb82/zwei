#include <cassert>
#include <chrono>
#include <algorithm>
#include <csignal>

#include <SDL_image.h>
#include <SDL_opengl.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl2.h>

#include <ASSETS/Assets.h>
#include <EMBEDDED/Font.h>

#include "./config.h"
#include "src/Gfx.h"
#include "src/Rt.h"

#include "src/ecs/Manager.h"
#include "src/ecs/Animation.h"
#include "src/ecs/Transform.h"
#include "src/ecs/Sprite.h"
#include "src/ecs/Acceleration.h"
#include "src/ecs/Controller.h"
#include "src/ecs/Collider.h"
#include "src/ecs/Ai.h"
#include "src/ecs/Attack.h"

#include "src/ecs/ui/Tweak.h"
#include "src/Map.h"
#include "src/in/Input.h"
#include "src/Col.h"

#include "src/ecs/minds/Skeleton.h"

void placeSkeleton(int x, int y, Topology &top) {
    auto skeleton = Manager::instance().addEntity(OBJECTS);
    skeleton->addComponent<Transform>(x, y);
    skeleton->addComponent<Sprite>(SPRITES);
    skeleton->addComponent<Animation>(200, true);
    skeleton->addComponent<Acceleration>(2.0f, 0);
    skeleton->addComponent<Ai>();

    skeleton->getComponent<Animation>()->addAnimationFrame(112, 64, 96, 80);
    skeleton->getComponent<Animation>()->addAnimationFrame(113, 65, 97, 81);
    skeleton->getComponent<Animation>()->addAnimationFrame(114, 66, 98, 82);

    skeleton->getComponent<Animation>()->stop();

    auto transform = skeleton->getComponent<Transform>();
    skeleton->addComponent<Collider>(transform, CT_ENEMY, 0.2, 0.4);
    top.registerMobile(&transform->p);

    auto ai = skeleton->getComponent<Ai>();
    ai->brainify<Skeleton>();
}

void loop() {
    Input in;
    SDL_Event event;
    GameKeyEvent controllerEvent;
    bool controllerFound = in.scan();
    if (!controllerFound) {
        return;
    }

    auto targetMillis = (1 / configTargetFramerate) * 1000;
    float millis = 0.0f;

    Map m("./assets/RAW");
    m.load("map.json");

    // Set map boundaries for the camera
    int mapSizeX, mapSizeY;
    m.getSize(FLOOR, &mapSizeX, &mapSizeY);
    RT_Camera.setMapSize(mapSizeX, mapSizeY);

    // Topology topology(mapSizeX, mapSizeY);
    RT_Context.getTopology().make(mapSizeX, mapSizeY);
    m.topology(RT_Context.getTopology().data());

    // Insert some sprite
    auto sprite = Manager::instance().addEntity(OBJECTS);
    RT_Context.setPlayer(sprite);

    sprite->addComponent<Transform>(2, 12);
    sprite->addComponent<Sprite>(SPRITES);
    sprite->addComponent<Animation>(200, true);
    sprite->addComponent<Acceleration>(3.0f, VM_25_PI);
    sprite->addComponent<Controller>();
    sprite->addComponent<Attack>();

    sprite->getComponent<Animation>()->addAnimationFrame(48, 0, 32, 16);
    sprite->getComponent<Animation>()->addAnimationFrame(49, 1, 33, 17);
    sprite->getComponent<Animation>()->addAnimationFrame(50, 2, 34, 18);
    sprite->getComponent<Animation>()->stop();
    // Track the sprite
    auto transform = sprite->getComponent<Transform>();
    RT_Camera.track(&transform->p);

    // Make the sprite collision aware
    sprite->addComponent<Collider>(transform, CT_PLAYER, 0.2, 0.4);

    auto tweakUi = Manager::instance().addEntity(UI);
    tweakUi->addComponent<Tweak>(sprite);

    placeSkeleton(13, 20, RT_Context.getTopology());
    placeSkeleton(15, 20, RT_Context.getTopology());
    placeSkeleton(13, 22, RT_Context.getTopology());
    placeSkeleton(15, 22, RT_Context.getTopology());

    while (RT_Running) {
        auto timeStart = std::chrono::system_clock::now();

        Manager::instance().collect();

        glClear(GL_COLOR_BUFFER_BIT);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                RT_Stop();
            }
            if (in.map(&event, &controllerEvent)) {
                Manager::instance().key(controllerEvent);
            }
        }

        Manager::instance().render(BACKGROUND);
        Manager::instance().render(FLOOR);
        Manager::instance().render(WALLS);
        Manager::instance().render(OBJECTS);
        Manager::instance().render(FOREGROUND);

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(Gfx_Window);
        ImGui::NewFrame();
        {
            Manager::instance().render(UI);
        }
        ImGui::Render();

        // RT_Camera.magnify(1.003);

        // Flush
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(Gfx_Window);
        glFinish();

        auto frameTime = std::chrono::system_clock::now() - timeStart;
        millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
        float delay = targetMillis - millis;

        if (delay > 0) {
            SDL_Delay(delay);
        }

        float dt = std::max(millis, delay);

        Col::collide(dt);
        Manager::instance().update(dt);
    }
}

void initStyles() {
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 2.0f;
}

void initImgui() {
    assert(Gfx_Window);
    assert(Gfx_Renderer);
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui_ImplSDL2_InitForOpenGL(Gfx_Window, Gfx_GL_Context);
    ImGui_ImplOpenGL2_Init();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Assets::instance().getFont(FONT), 16, 16);
}

void initAssets() {
    assert(Gfx_Renderer);

    Assets::instance().addFont(FONT, assets_Font);
    // Assets::instance().addTexture(TILES, assets_Tiles);
    Assets::instance().addTexture(TILES, "assets/RAW/tiles.png");
    Assets::instance().addTexture(SPRITES, "assets/RAW/sprites.png");
}

void initSdl() {

    auto sdlFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;

    if (SDL_Init(sdlFlags) != 0) {
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetSwapInterval(1);

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        exit(1);
    }

    Gfx_Window = SDL_CreateWindow(
            configWindowTitle,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            configWindowWidth,
            configWindowHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );

    Gfx_Renderer = SDL_CreateRenderer(Gfx_Window, -1, SDL_RENDERER_ACCELERATED);
    Gfx_GL_Context = SDL_GL_CreateContext(Gfx_Window);
    SDL_GL_MakeCurrent(Gfx_Window, Gfx_GL_Context);
    Gfx_Tile_Size = configTileSize;
}

void onSignal(int sig) {
    exit(sig);
}

int main(int, char **) {
    signal(SIGABRT, onSignal);
    signal(SIGTERM, onSignal);
    signal(SIGINT, onSignal);
    signal(SIGSEGV, onSignal);

    // Seed random number generator
    srand((unsigned int) time(nullptr));

    initSdl();
    initAssets();
    initImgui();
    initStyles();
    loop();
}
