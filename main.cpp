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
#include "src/ecs/Stats.h"
#include "src/ecs/Menu.h"

#include "src/ecs/ui/Tweak.h"
#include "src/Map.h"
#include "src/in/Input.h"
#include "src/Col.h"

#include "src/ecs/minds/Kakta.h"
#include "src/ecs/Analytics.h"
#include "src/ecs/arms/Stick.h"
#include "src/ecs/minds/Spider.h"
#include "src/ecs/Collectable.h"
#include "src/snd/Player.h"
#include "src/alg/Text.h"
#include "src/Draw.h"
#include "src/St.h"

float targetMillis = (1 / St::instance().getFps()) * 1000;

void placeKakta(int x, int y, Topology &top) {
    auto kakta = Manager::instance().addEntity(OBJECTS);
    kakta->addComponent<Transform>(x, y);
    kakta->addComponent<Sprite>(SPRITES);
    kakta->addComponent<Animation>(200, true);
    kakta->addComponent<Acceleration>(2.0f, 0);
    kakta->addComponent<Ai>();
    kakta->addComponent<Attack>();

    kakta->getComponent<Animation>()->addAnimationFrame(112, 64, 96, 80);
    kakta->getComponent<Animation>()->addAnimationFrame(113, 65, 97, 81);
    kakta->getComponent<Animation>()->addAnimationFrame(114, 66, 98, 82);

    kakta->getComponent<Animation>()->addAttackFrame(115, 67, 99, 83, 300);

    kakta->getComponent<Animation>()->stop();
    kakta->addComponent<Analytics>();

    kakta->addComponent<Attack>();

    kakta->addComponent<Stats>(false);
    auto stats = kakta->getComponent<Stats>();
    stats->inventory.equip(std::make_shared<Stick>());
    stats->character.setBase(5, 10, 1, 1);
    stats->character.setLevel(1);

    auto transform = kakta->getComponent<Transform>();
    kakta->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 1, 0});
    top.registerMobile(&transform->p);

    auto ai = kakta->getComponent<Ai>();
    ai->brainify<Kakta>();
}

void placeSpider(int x, int y, Topology &top) {
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
    stats->character.setLevel(1);
    stats->character.setBase(10, 5, 1, 5);

    auto transform = skeleton->getComponent<Transform>();
    skeleton->addComponent<Collider>(transform, CT_ENEMY, Padding{.5, .5, 0.3, 0.7});
    top.registerMobile(&transform->p);

    auto ai = skeleton->getComponent<Ai>();
    ai->brainify<Spider>();
}

void placeItem(float x, float y, ItemType type) {
    auto entity = Item::make({x, y}, type);
    Manager::instance().enqueue(entity, ITEMS);
}

typedef decltype(std::chrono::system_clock::now()) tp;

void renderMenu(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(Gfx_Window);
    ImGui::NewFrame();
    {
        Manager::instance().render(UI);
    }
    ImGui::Render();

    // Flush
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(Gfx_Window);
    glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;
    if (delay > 0) SDL_Delay(delay);
}

void renderGameOver(tp frameStart, float *darkness) {
    auto t1 = Assets::instance().getTexture(TILES);

    SDL_SetTextureColorMod(t1->mem, 255, *darkness, *darkness);
    SDL_SetTextureAlphaMod(t1->mem, *darkness);

    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(FOREGROUND);

    auto texture = Assets::instance().getTexture(SPRITES);
    std::string s("game over");

    SDL_Rect target;
    target.x = (configWindowWidth / 2) - ((s.length() * 24) / 2);
    target.y = (configWindowHeight / 2) - 12;
    target.w = 32;
    target.h = 32;

    for (const char c : s) {
        SDL_Rect source;
        Gfx::pick(source, Text::fromChar(c), texture->w);
        Draw::instance().draw(texture->mem, source, target);
        target.x += 24;
    }

    RT_Camera.magnify(0.999);

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    glFinish();

    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;

    if (delay > 0) {
        SDL_Delay(delay);
    }

    float dt = std::max(millis, delay);
    Manager::instance().update(dt);

    *darkness -= 0.5f;
    if (*darkness <= 0) *darkness = 0.0f;
}

void renderGame(tp frameStart) {
    Manager::instance().render(BACKGROUND);
    Manager::instance().render(FLOOR);
    Manager::instance().render(WALLS);
    Manager::instance().render(ITEMS);
    Manager::instance().render(OBJECTS);
    Manager::instance().render(FOREGROUND);

    // Flush
    SDL_GL_SwapWindow(Gfx_Window);
    glFinish();

    // Frametime
    auto frameTime = std::chrono::system_clock::now() - frameStart;
    float millis = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime).count();
    float delay = targetMillis - millis;
    if (delay > 0) SDL_Delay(delay);
    float dt = std::max(millis, delay);

    // Update entities
    Col::collide(dt);
    Manager::instance().update(dt);
}

void loop() {
    Input in;
    SDL_Event event;
    GameKeyEvent gk;
    bool controllerFound = in.scan();
    if (!controllerFound) {
        // return;
    }

    Player::instance().playMusic(MUSIC_1);

    Map m("./assets/RAW");
    m.load("dungeon.json");

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

    sprite->addComponent<Transform>(7, 14);
    sprite->addComponent<Sprite>(SPRITES);
    sprite->addComponent<Animation>(200, true);
    sprite->addComponent<Acceleration>(3.0f, VM_25_PI);
    sprite->addComponent<Controller>();
    sprite->addComponent<Attack>();
    sprite->addComponent<Analytics>();
    sprite->addComponent<Stats>(true);

    sprite->getComponent<Animation>()->addAnimationFrame(48, 0, 32, 16);
    sprite->getComponent<Animation>()->addAnimationFrame(49, 1, 33, 17);
    sprite->getComponent<Animation>()->addAnimationFrame(50, 2, 34, 18);

    sprite->getComponent<Animation>()->addAttackFrame(51, 3, 35, 19, 100);
    sprite->getComponent<Animation>()->addAttackFrame(52, 4, 36, 20, 100);
    sprite->getComponent<Animation>()->addAttackFrame(53, 5, 37, 21, 100);

    sprite->getComponent<Animation>()->stop();
    // Track the sprite
    auto transform = sprite->getComponent<Transform>();
    RT_Camera.track(&transform->p);

    // Make the sprite collision aware
    sprite->addComponent<Collider>(transform, CT_PLAYER,
                                   Padding{.3, .3, 0.6, 0});

    auto tweakUi = Manager::instance().addEntity(UI);
    // tweakUi->addComponent<Tweak>(sprite);

    auto stats = sprite->getComponent<Stats>();
    stats->character.setBase(10, 1, 1, 1);


    /*
    placeKakta(11, 11, RT_Context.getTopology());
    placeKakta(10, 10, RT_Context.getTopology());
    placeKakta(12, 8, RT_Context.getTopology());
    placeSpider(8, 8, RT_Context.getTopology());

    placeItem(8, 8, HEALTH_POTION);
    placeItem(9, 8, STICK);
    placeItem(9, 5, STICK);
    placeItem(9, 6, HEALTH_POTION);
    */

    // Global alpha
    float ga = 255.0f;

    while (RT_Running) {
        auto frameStart = std::chrono::system_clock::now();

        Manager::instance().collect();

        glClear(GL_COLOR_BUFFER_BIT);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                St::instance().serialize();
                RT_Stop();
                continue;
            }
            if (in.map(&event, &gk)) {
                if (gk.valid && gk.state == GK_PUSHED && gk.key == GK_START) {
                    Rt::instance().context.state.toggleMenu();
                } else {
                    if (RT_Context.state.currentState() == MainMenu) {
                        Manager::instance().uiInput(gk);
                    } else {
                        Manager::instance().key(gk);
                    }
                }
            }
        }

        switch (RT_Context.state.currentState()) {
            case Game:
                ga = 255.0f;
                renderGame(frameStart);
                break;
            case GameOver:
                renderGameOver(frameStart, &ga);
                break;
            case MainMenu:
                renderMenu(frameStart);
                break;
            default:
                continue;
        }
    }
}

void initStyles() {
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowRounding = 3.0f;
}

void initSound() {
    // force sound to init
    Player::instance();
    St::instance().initAll();
}

void initMenu() {
    auto entity = Manager::instance().addEntity(UI);
    entity->addComponent<Menu>();
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

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowBorderSize = 4.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4{0.345, 0.345, 0.98, 0.4};
    style.Colors[ImGuiCol_Border] = ImVec4{0.949, 0.949, 0.949, 1};
    style.Colors[ImGuiCol_Text] = ImVec4{0.949, 0.949, 0.949, 1};
    style.Colors[ImGuiCol_Button] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_ButtonActive] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4{0.1, 0.1, 0.1, 0};
    style.Colors[ImGuiCol_Separator] = ImVec4{0.1, 0.1, 0.1, 0};
}

void initAssets() {
    assert(Gfx_Renderer);

    Assets::instance().addFont(FONT, assets_Font);
    // Assets::instance().addTexture(TILES, assets_Tiles);
    Assets::instance().addTexture(TILES, "assets/RAW/sprites.png");
    Assets::instance().addTexture(SPRITES, "assets/RAW/sprites.png");
}

void initSdl() {

    auto sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                    SDL_INIT_GAMECONTROLLER;

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
    initSound();
    initMenu();
    loop();
}
