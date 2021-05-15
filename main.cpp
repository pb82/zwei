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
#include "src/ecs/Animation.h"
#include "src/ecs/Transform.h"
#include "src/ecs/Sprite.h"
#include "src/ecs/Acceleration.h"
#include "src/ecs/Collider.h"
#include "src/ecs/Ai.h"
#include "src/ecs/Attack.h"
#include "src/ecs/Stats.h"
#include "src/ecs/Menu.h"
#include "src/ecs/Trigger.h"
#include "src/ecs/Hud.h"

#include "src/ecs/ui/Tweak.h"
#include "src/Map.h"
#include "src/in/Input.h"
#include "src/Col.h"

#include "src/ecs/minds/Kakta.h"
#include "src/ecs/Analytics.h"
#include "src/ecs/arms/Stone.h"
#include "src/ecs/minds/Spider.h"
#include "src/ecs/Collectable.h"
#include "src/snd/Player.h"
#include "src/alg/Text.h"
#include "src/Draw.h"
#include "src/St.h"
#include "src/ecs/Bar.h"

#include "src/Api.h"
#include "src/ecs/arms/Stick.h"
#include "src/ecs/Controller.h"

float targetMillis = (1 / St::instance().getFps()) * 1000;
std::string game_over("game over");
std::string saving_game("saving game...");
std::string loading_game("loading game...");
Color blackbox{0, 0, 0, 0};
int globalFrameCounter = 0;
int artificialDelay = St::instance().getFps();

void placeTrigger(int x, int y, trigger_Fn enter, trigger_Fn exit) {
    auto trigger = Manager::instance().addEntity(FLOOR);
    trigger->addComponent<Transform>(x, y);

    auto transform = trigger->getComponent<Transform>();
    trigger->addComponent<Collider>(transform, CT_TRIGGER);
    trigger->addComponent<Trigger>();

    auto handler = trigger->getComponent<Trigger>();
    handler->onEnter(enter);
    handler->onExit(exit);
}

void placeSpider(int x, int y) {
    auto skeleton = Manager::instance().addEntity(OBJECTS);
    skeleton->addComponent<Transform>(x, y);
    skeleton->addComponent<Sprite>(SPRITES);
    skeleton->addComponent<Animation>(200, true);
    skeleton->addComponent<Acceleration>(4.0f, 0);

    skeleton->addComponent<Ai>();
    skeleton->addComponent<Attack>();

    skeleton->getComponent<Animation>()->addAnimationFrame(144);
    skeleton->getComponent<Animation>()->addAnimationFrame(145);
    skeleton->getComponent<Animation>()->addAnimationFrame(146);
    skeleton->getComponent<Animation>()->stop();
    skeleton->addComponent<Analytics>();

    skeleton->addComponent<Stats>(false);
    auto stats = skeleton->getComponent<Stats>();
    stats->character.setBase(10, 1, 1, 10);

    auto transform = skeleton->getComponent<Transform>();
    skeleton->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.3, 0.7});
    RT_Topology.registerMobile(&transform->p);

    auto ai = skeleton->getComponent<Ai>();
    ai->brainify<Spider>();
}

typedef decltype(std::chrono::system_clock::now()) tp;

void renderMenu(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ROOF);
    Manager::instance().render(SKY);

    ImGui_ImplSDL2_NewFrame(Gfx_Window);
    ImGui::NewFrame();
    {
        Manager::instance().render(UI);
    }
    ImGui::Render();

    // Flush
    ImGuiSDL::Render(ImGui::GetDrawData());
    SDL_GL_SwapWindow(Gfx_Window);
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;
    if (delay > 0) SDL_Delay(delay);
}

void renderLoad(tp frameStart) {
    auto texture = Assets::instance().getTexture(BITMAPFONT);
    SDL_SetRenderDrawColor(Gfx_Renderer, 0, 0, 255, 255);

    // Loading a saved game
    if (globalFrameCounter < artificialDelay) {
        if (globalFrameCounter < 1) {
            // First flush all entities
            Manager::instance().resetAll();
            RT_Context.setPlayer(nullptr);
        } else if (globalFrameCounter < 2) {
            Api::init();
        } else if (globalFrameCounter < 3) {
            auto s = RT_State.currentState();
            float x = 0, y = 0;
            RT_Context.load(&x, &y);
            Api::setPlayerPosition(x, y);
            RT_State.pushState(s);
        }
    } else {
        RT_State.popState();
        RT_State.pushState(StateGame);
    }

    SDL_Rect target;
    target.x = (configWindowWidth / 2) - ((loading_game.length() * 24) / 2);
    target.y = (configWindowHeight / 2) - 12;
    target.w = 32;
    target.h = 32;

    for (const char c : loading_game) {
        SDL_Rect source;
        Gfx::pickText(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    target.x = (configWindowWidth / 2) - (200 / 2);
    target.y = (configWindowHeight / 2) + 32;
    target.w = 200;
    target.h = 32;

    Draw::instance().rect(color_White, target);

    target.x += 1;
    target.y += 1;

    float w = (100 / (float) artificialDelay) * (float) globalFrameCounter;
    target.w = static_cast<int>(w * 2);
    target.h -= 2;

    Draw::instance().box(color_Blue, target);

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;

    if (delay > 0) {
        SDL_Delay(delay);
    }
    globalFrameCounter++;
}

void renderSave(tp frameStart) {
    auto texture = Assets::instance().getTexture(BITMAPFONT);
    SDL_SetRenderDrawColor(Gfx_Renderer, 0, 0, 255, 255);

    std::string message;
    if (globalFrameCounter < artificialDelay) {
        if (globalFrameCounter == 0) {
            float x, y;
            Api::getPlayerPosition(x, y);
            RT_Context.save(x, y);
        }
        message = saving_game;
    } else {
        RT_State.popState();
        RT_State.pushState(StateGame);
        RT_Player->getComponent<Controller>()->resetKeys();
        RT_Player->getComponent<Acceleration>()->decelerate();
        RT_Player->getComponent<Animation>()->stop();
        Player::instance().resume();
    }

    SDL_Rect target;
    target.x = (configWindowWidth / 2) - ((message.length() * 24) / 2);
    target.y = (configWindowHeight / 2) - 12;
    target.w = 32;
    target.h = 32;

    for (const char c : message) {
        SDL_Rect source;
        Gfx::pickText(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    target.x = (configWindowWidth / 2) - (200 / 2);
    target.y = (configWindowHeight / 2) + 32;
    target.w = 200;
    target.h = 32;

    Draw::instance().rect(color_White, target);

    target.x += 1;
    target.y += 1;

    float w = (100 / (float) artificialDelay) * (float) globalFrameCounter;
    target.w = static_cast<int>(w * 2);
    target.h -= 2;

    Draw::instance().box(color_Blue, target);

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;

    if (delay > 0) {
        SDL_Delay(delay);
    }
    globalFrameCounter++;
}

void renderGameOver(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ROOF);
    Manager::instance().render(SKY);
    Manager::instance().render(FOREGROUND);

    auto texture = Assets::instance().getTexture(BITMAPFONT);

    SDL_Rect target;

    target.x = 0;
    target.y = 0;
    target.w = configWindowWidth;
    target.h = configWindowHeight;

    Draw::instance().box(blackbox, target);
    if (blackbox.a < 255) {
        blackbox.a++;
    }

    target.x = (configWindowWidth / 2) - ((game_over.length() * 24) / 2);
    target.y = (configWindowHeight / 2) - 12;
    target.w = 32;
    target.h = 32;

    for (const char c : game_over) {
        SDL_Rect source;
        Gfx::pickText(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;

    if (delay > 0) {
        SDL_Delay(delay);
    }

    float dt = std::max(millis, delay);
    Manager::instance().update(dt);
}

void renderGame(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ITEMS);
    Manager::instance().render(OBJECTS);
    Manager::instance().render(ROOF);
    Manager::instance().render(SKY);
    Manager::instance().render(FOREGROUND);

    if (!Rt_Commands.empty()) {
        Rt_Commands.front()->render();
        if (Rt_Commands.front()->done()) {
            Rt_Commands.pop();
        }
    }

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    SDL_RenderPresent(Gfx_Renderer);
    // glFinish();

    // Frametime
    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;
    if (delay > 0) SDL_Delay(delay);
    float dt = std::max(millis, delay);

    // Update entities
    if (!Rt_Commands.empty()) {
        Rt_Commands.front()->update(dt);
    } else {
        Manager::instance().update(dt);
        Col::collide(dt);
    }
}

void loop() {
    SDL_Event event;
    GameKeyEvent gk;
    bool controllerFound = RT_Input.scan();
    if (!controllerFound) {
        // return;
    }

    RT_Context.setActiveScene(SceneStart);

    // Global alpha
    while (RT_Running) {
        auto frameStart = std::chrono::system_clock::now();

        Manager::instance().collect();

        SDL_SetRenderDrawColor(Gfx_Renderer, 47, 40, 58, 255);
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
            case StateGame:
                blackbox.a = 0;
                globalFrameCounter = 0;
                renderGame(frameStart);
                break;
            case StateGameOver:
                renderGameOver(frameStart);
                break;
            case StateMainMenu:
            case StateStart:
                renderMenu(frameStart);
                break;
            case StateSaving:
                renderSave(frameStart);
                break;
            case StateLoading:
                renderLoad(frameStart);
                break;
            default:
                continue;
        }
    }
}

void initSound() {
    // force sound to init
    Player::instance();
    St::instance().initAll();
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
    Assets::instance().addTexture(TILES_DUNGEON, "assets/RAW/dungeon_sheet.png");
    Assets::instance().addTexture(TILES_BEACH, "assets/RAW/beach.png");
    Assets::instance().addTexture(SPRITES, "assets/RAW/sprites.png");
    Assets::instance().addTexture(BITMAPFONT, "assets/RAW/bitmapfont.png");
}

void initSdl() {

    auto sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                    SDL_INIT_GAMECONTROLLER;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_INIT_GAMECONTROLLER
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
    initSound();
    loop();

    ImGuiSDL::Deinitialize();
}
