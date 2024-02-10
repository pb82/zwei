#include <cassert>
#include <chrono>
#include <algorithm>
#include <csignal>

#include <SDL_image.h>
#include <SDL_mixer.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_sdl.h>

#include <ASSETS/Assets.h>
#include <EMBEDDED/Font.h>

#include "./config.h"
#include "src/Gfx.h"
#include "src/Rt.h"

#include "src/ecs/Manager.h"
#include "src/ecs/Transform.h"
#include "src/ecs/Collider.h"
#include "src/ecs/Menu.h"
#include "src/in/Input.h"
#include "src/St.h"

#include "src/Api.h"

float targetMillis = (1 / St::instance().getFps()) * 1000;
typedef decltype(std::chrono::system_clock::now()) tp;

void renderMenu(tp frameStart) {
    ImGui_ImplSDL2_NewFrame(Gfx_Window);
    ImGui::NewFrame();
    {
        Manager::instance().render(UI);
    }
    ImGui::Render();

    // Flush
    ImGuiSDL::Render(ImGui::GetDrawData());
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;
    if (delay > 0) SDL_Delay(delay);
}

void loop() {
    SDL_Event event;
    GameKeyEvent gk;
    bool controllerFound = RT_Input.scan();
    if (!controllerFound) {
        // return;
    }

    RT_Context.setActiveScene(Scene_Game);

    while (RT_Running) {
        auto frameStart = std::chrono::system_clock::now();

        Manager::instance().collect();

        SDL_SetRenderDrawColor(Gfx_Renderer, 0, 0, 0, 255);
        SDL_RenderClear(Gfx_Renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                St::instance().serialize();
                RT_Stop();
                continue;
            }
            if (RT_Input.map(&event, &gk)) {
                gk.source = &event;

                if (!Rt_Commands.empty()) {
                    Rt_Commands.front()->key(gk);
                    // Accept release events, otherwise the button
                    // stays pushed even when the command is cleared
                    if (gk.state == GK_RELEASED) Manager::instance().key(gk);
                } else if (gk.state == GK_PUSHED && gk.key == GK_START) {
                    if (RT_State.toggleMenu()) {
                        auto m = RT_Menu->getComponent<Menu>();
                        m->resetMenu();
                    }
                } else {
                    if (RT_State.currentState() == StateMainMenu ||
                        RT_State.currentState() == StateStart) {
                        Manager::instance().uiInput(gk);
                    } else {
                        Manager::instance().key(gk);
                    }
                }
            }
        }

        switch (RT_Context.state.currentState()) {
            case StateStart:
            case StateMainMenu:
                renderMenu(frameStart);
                break;
            case StateGame:
                break;
        }
    }
}

void initImgui() {
    assert(Gfx_Window);
    assert(Gfx_Renderer);
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::GetIO().Fonts->AddFontFromMemoryTTF(Assets::instance().getFont(FONT), 16, 16);

    ImGuiStyle &style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4{0.345, 0.345, 0.98, 0.4};
    style.Colors[ImGuiCol_Text] = ImVec4{0.949, 0.949, 0.949, 1};
    style.Colors[ImGuiCol_Button] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_ButtonActive] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_Separator] = ImVec4{0.1, 0.1, 0.1, 0};

    ImGuiStyle &s = ImGui::GetStyle();
    ImGuiSDL::Initialize(Gfx_Renderer, configWindowWidth, configWindowHeight);
}

void initAssets() {
    assert(Gfx_Renderer);

    Assets::instance().addFont(FONT, assets_Font);
    // Assets::instance().addTexture(TILES, assets_Tiles);
    Assets::instance().addTexture(TILES_BEACH, "assets/SIMPLE/TILESETS/Tiles.png");
    // Assets::instance().addTexture(GIRL, "assets/NEW/TILESETS/Girl.png");
    // Assets::instance().addTexture(CATERPILLAR, "assets/NEW/TILESETS/Caterpillar.png");
    Assets::instance().addTexture(SPRITES, "assets/RAW/sprites.png");
    Assets::instance().addTexture(BITMAPFONT, "assets/RAW/bitmapfont.png");
}

void initSdl() {
    auto sdlFlags =
            SDL_INIT_VIDEO |
            SDL_INIT_AUDIO |
            SDL_INIT_TIMER |
            SDL_INIT_EVENTS |
            SDL_INIT_JOYSTICK |
            SDL_INIT_GAMECONTROLLER;

    // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetSwapInterval(1);

    if (SDL_Init(sdlFlags) != 0) {
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        exit(1);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
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

    // Deal with high dpi displays where the scaled resolution is not the same as
    // the requested resolution
    int actualW, actualH;
    SDL_GL_GetDrawableSize(Gfx_Window, &actualW, &actualH);
    configRenderScaleX = actualW / configWindowWidth;
    configRenderScaleY = actualH / configWindowHeight;

    Gfx_Renderer = SDL_CreateRenderer(Gfx_Window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(Gfx_Renderer, configRenderScaleX, configRenderScaleY);
    Gfx_Tile_Size = configTileSize;
}

void onSignal(int sig) {
    ImGuiSDL::Deinitialize();
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
    loop();

    ImGuiSDL::Deinitialize();
}
