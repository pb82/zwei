#include <cassert>
#include <chrono>
#include <algorithm>
#include <csignal>

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <ASSETS/Assets.h>

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
#include "src/Bus.h"
#include "src/Lighting.h"
#include "src/ecs/SelfDestruct.h"
#include "src/ecs/arms/Stick.h"
#include "src/ecs/Controller.h"

float targetMillis = (1 / St::instance().getFps()) * 1000;
std::string game_over("game over");
std::string saving_game("saving game...");
std::string loading_game("loading game...");
Color blackbox{0, 0, 0, 0};
int globalFrameCounter = 0;
int artificialDelay = St::instance().getFps();

typedef decltype(std::chrono::system_clock::now()) tp;

void renderMenu(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ROOF);
    Manager::instance().render(SKY);

    Manager::instance().render(UI);

    // Flush
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

    for (const char c: loading_game) {
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
    SDL_RenderPresent(Gfx_Renderer);

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

    for (const char c: game_over) {
        SDL_Rect source;
        Gfx::pickText(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    // Flush
    SDL_RenderPresent(Gfx_Renderer);

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
    Manager::instance().render(PARALLAX);
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ITEMS);
    Manager::instance().render(OBJECTS);
    Manager::instance().render(ROOF);
    Manager::instance().render(SKY);
    Lighting::instance().renderDarkness();
    Manager::instance().render(FOREGROUND);

    if (!Rt_Commands.empty()) {
        Rt_Commands.front()->render();
        if (Rt_Commands.front()->done()) {
            Rt_Commands.pop();
        }
    }

    // Flush
    // SDL_GL_SwapWindow(Gfx_Window);
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
        Lighting::instance().update(dt);
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

    RT_Context.setActiveScene(SceneEntry);

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
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F5) {
                RT_Context.reloadActiveScene();
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
                        Player::instance().pause();
                        auto m = RT_Menu->getComponent<Menu>();
                        m->resetMenu();
                    } else {
                        Player::instance().resume();
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
            case StateLoading:
                renderLoad(frameStart);
                break;
            default:
                continue;
        }
    }
}

void initBus() {
    Bus::instance().subscribe(EventStateChangeRequested, [](const Event &e) {
        const auto &evt = static_cast<const StateChangeRequestedEvent &>(e);
        RT_State.pushState(static_cast<GameState>(evt.target));
    });
    Bus::instance().subscribe(EventQuit, [](const Event &) {
        RT_Running = false;
    });
    Bus::instance().subscribe(EventPlayerDied, [](const Event &) {
        Bus::instance().publish(StateChangeRequestedEvent(StateGameOver));
        Player::instance().playMusic(MUSIC_GAMEOVER);
    });
    Bus::instance().subscribe(EventItemCollected, [](const Event &) {
        Player::instance().playSound(SOUND_PICKUP);
    });
    Bus::instance().subscribe(EventEnemyDied, [](const Event &e) {
        const auto &evt = static_cast<const EnemyDiedEvent &>(e);
        auto entity = std::make_shared<Entity>();
        entity->addComponent<Sprite>(SPRITES);
        entity->addComponent<Animation>(100, false);
        auto explosion = entity->getComponent<Animation>();
        explosion->addAnimationFrame(128);
        explosion->addAnimationFrame(129);
        explosion->addAnimationFrame(130);
        explosion->addAnimationFrame(131);
        entity->addComponent<SelfDestruct>(TIMER, 400);
        entity->addComponent<Transform>(evt.x, evt.y);
        Manager::instance().enqueue(entity, FOREGROUND);
    });
}

void initSound() {
    // force sound to init
    Player::instance();
    St::instance().initAll();
}

void initAssets() {
    assert(Gfx_Renderer);

    // Assets::instance().addTexture(TILES, assets_Tiles);
    Assets::instance().addTexture(TILES_BEACH, "assets/NEW/TILESETS/Tiles.png");
    Assets::instance().addTexture(GIRL, "assets/NEW/TILESETS/Girl.png");
    Assets::instance().addTexture(CATERPILLAR, "assets/NEW/TILESETS/Caterpillar.png");
    Assets::instance().addTexture(SPRITES, "assets/RAW/sprites.png");
    Assets::instance().addTexture(BITMAPFONT, "assets/RAW/bitmapfont.png");
}

void initSdl() {

    auto sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                    SDL_INIT_GAMECONTROLLER;

    if (SDL_Init(sdlFlags) != 0) {
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        exit(1);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        exit(1);
    }

    if (TTF_Init() != 0) {
        exit(1);
    }

    Gfx_Window = SDL_CreateWindow(
            configWindowTitle,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            configWindowWidth,
            configWindowHeight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI // || SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    // In fullscreen desktop mode, get the actual window size
    SDL_GetWindowSize(Gfx_Window, &configWindowWidth, &configWindowHeight);
    configZoomFactor = (float)configWindowWidth / (float)configVirtualWidth;

    Gfx_Renderer = SDL_CreateRenderer(Gfx_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Deal with high dpi displays where the scaled resolution is not the same as
    // the requested resolution
    int actualW, actualH;
    SDL_GetRendererOutputSize(Gfx_Renderer, &actualW, &actualH);
    configRenderScaleX = (float)actualW / (float)configWindowWidth;
    configRenderScaleY = (float)actualH / (float)configWindowHeight;

    SDL_RenderSetScale(Gfx_Renderer, configRenderScaleX, configRenderScaleY);
    Gfx_Tile_Size = configTileSize;

    // Update camera zoom to match fullscreen resolution
    RT_Camera.z = configZoomFactor;
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
    initSound();
    initBus();
    loop();
}
