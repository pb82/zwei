#include <IMGUI/imgui.h>
#include "Menu.h"

#include "../Rt.h"

#define WINDOW_MARGIN 0

static ImVec4 white() {
    return ImVec4{1, 1, 1, 1};
}

static ImVec4 black() {
    return ImVec4{0, 0, 0, 1};
}

static ImVec4 grey() {
    return ImVec4{0.3, 0.3, 0.3, 1};
}

static ImVec4 lightgrey() {
    return ImVec4{0.5, 0.5, 0.5, 1};
}

static void getWinSize(ImVec2 &s) {
    s.x = configWindowWidth - WINDOW_MARGIN;
    s.y = configWindowHeight - WINDOW_MARGIN;
}

static void getWinPos(ImVec2 &p) {
    p.x = WINDOW_MARGIN / 2;
    p.y = WINDOW_MARGIN / 2;
}

static int getWinFlags() {
    return ImGuiWindowFlags_NoDecoration
           | ImGuiWindowFlags_NoResize
           | ImGuiWindowFlags_NoMove
           | ImGuiWindowFlags_NoCollapse;
}

static ImVec2 getItemSize() {
    ImVec2 size;
    size.x = configWindowWidth / 2;
    size.y = 32;
    return size;
}

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

    allItems.emplace(ItemNewGame, std::make_shared<MenuItem>("New Game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_Context.setActiveScene(SceneForest);
        }
    }));

    allItems.emplace(ItemSave, std::make_shared<MenuItem>("Save Game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_State.pushState(StateSaving);
        }
    }));

    allItems.emplace(ItemLoad, std::make_shared<MenuItem>("Load game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_State.pushState(StateLoading);
        }
    }));

    allItems.emplace(ItemContinue, std::make_shared<MenuItem>("Continue", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_State.pushState(StateLoading);
        }
    }));

    allItems.emplace(ItemSettings, std::make_shared<MenuItem>("Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Settings);
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemBack, std::make_shared<MenuItem>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemAudioSettings, std::make_shared<MenuItem>("Audio Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(AudioSettings);
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemMusicVolume,
                     std::make_shared<MenuItem>("Music Volume (%)", &this->musicVolume, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decMusicVolume();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incMusicVolume();
                         }
                     }));

    allItems.emplace(ItemEffectsVolume,
                     std::make_shared<MenuItem>("Effects Volume (%)", &this->effectsVolume, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decEffectsVolume();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incEffectsVolume();
                         }
                     }));

    allItems.emplace(ItemFps,
                     std::make_shared<MenuItem>("Target FPS", &this->currentFps, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decFps();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incFps();
                         }
                     }));

    allItems.emplace(ItemWindowSize,
                     std::make_shared<MenuItem>("Window Size", &this->windowSize, [](GameKeyEvent &key) {
                         if (key.key == GK_LEFT) {
                             St::instance().decWindowSize();
                         } else if (key.key == GK_RIGHT) {
                             St::instance().incWindowSize();
                         }
                     }));

    allItems.emplace(ItemVideoSettings, std::make_shared<MenuItem>("Video Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(VideoSettings);
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemKeyboardSettings, std::make_shared<MenuItem>("Keyboard", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Keyboard);
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemGamepadSettings, std::make_shared<MenuItem>("Gamepad", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Gamepad);
        this->selectedIndex = 0;
    }));

    allItems.emplace(ItemQuit, std::make_shared<MenuItem>("Quit", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(AreYouSure);
        this->selectedIndex = 1;
    }));

    allItems.emplace(ItemAreYouSure, std::make_shared<MenuItem>("Are you sure?"));

    allItems.emplace(ItemYes, std::make_shared<MenuItem>("Yes", [](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        St::instance().serialize();
        RT_Running = false;
    }));

    allItems.emplace(ItemNo, std::make_shared<MenuItem>("No", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));


}

void Menu::render(uint8_t) {
    this->currentFps = std::to_string(static_cast<int>(St::instance().getFps()));
    this->musicVolume = std::to_string(St::instance().getMusicVolume());
    this->effectsVolume = std::to_string(St::instance().getEffectsVolume());
    St::instance().getWindowSize(this->windowSize);

    buildMenu();

    ImVec2 size;
    getWinSize(size);

    ImVec2 position;
    getWinPos(position);

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Menu", nullptr, getWinFlags());

    for (int i = 0; i < items.size(); i++) {
        auto &item = items.at(i);
        items.at(i)->render(i == selectedIndex);
    }

    ImGui::End();
}

void Menu::up() {
    for (int i = selectedIndex - 1; i >= 0; --i) {
        if (items.at(i)->canSelect) {
            selectedIndex = i;
            break;
        }
    }
}

void Menu::down() {
    for (int i = selectedIndex + 1; i < items.size(); ++i) {
        if (items.at(i)->canSelect) {
            selectedIndex = i;
            break;
        }
    }
}

void Menu::key(GameKeyEvent &key) {
    if (!key.valid) return;
    if (key.state != GK_PUSHED) return;
    if (key.key == GK_B && menuState != AwaitBinding) {
        if (this->level.size() > 1) this->level.pop();
        this->selectedIndex = 0;
    } else if (key.key == GK_UP && menuState != AwaitBinding) {
        up();
    } else if (key.key == GK_DOWN && menuState != AwaitBinding) {
        down();
    } else {
        this->items.at(selectedIndex)->call(key);
    }
}

void Menu::resetMenu() {
    while (!this->level.empty()) this->level.pop();
    this->level.push(Main);
    this->selectedIndex = 0;
}

void Menu::buildStartMenu(bool started) {
    switch (this->level.top()) {
        case Main:
            if (!started) {
                this->items.push_back(allItems.at(ItemNewGame));
            } else {
                this->items.push_back(allItems.at(ItemSave));
            }
            if (RT_Context.savegameExists()) {
                if (!started) {
                    this->items.push_back(allItems.at(ItemContinue));
                } else {
                    this->items.push_back(allItems.at(ItemLoad));
                }
            }
            this->items.push_back(allItems.at(ItemSettings));
            this->items.push_back(allItems.at(ItemQuit));
            return;
        case Settings:
            this->items.push_back(allItems.at(ItemAudioSettings));
            this->items.push_back(allItems.at(ItemVideoSettings));
            this->items.push_back(allItems.at(ItemKeyboardSettings));
            this->items.push_back(allItems.at(ItemGamepadSettings));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case AreYouSure:
            this->items.push_back(allItems.at(ItemAreYouSure));
            this->items.push_back(allItems.at(ItemYes));
            this->items.push_back(allItems.at(ItemNo));
            return;
        case AudioSettings:
            this->items.push_back(allItems.at(ItemMusicVolume));
            this->items.push_back(allItems.at(ItemEffectsVolume));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case VideoSettings:
            this->items.push_back(allItems.at(ItemWindowSize));
            this->items.push_back(allItems.at(ItemFps));
            this->items.push_back(allItems.at(ItemBack));
            return;
        case Keyboard:
            populateKeyboardMenu();
            this->items.push_back(allItems.at(ItemBack));
            return;
        case Gamepad:
            populateGamepadMenu();
            this->items.push_back(allItems.at(ItemBack));
            return;
        default:
            return;
    }
}

void Menu::buildMenu() {
    this->items.clear();

    auto state = RT_State.currentState();

    switch (state) {
        case StateStart:
            buildStartMenu(false);
            return;
        case StateMainMenu:
            buildStartMenu(true);
            return;
        default:
            return;
    }
}

void Menu::addGamepadMenuItem(GameKey k, SDL_GameControllerButton button) {
    auto bound = Input::toString(k);
    auto b = SDL_GameControllerGetStringForButton(button);
    items.push_back(
            std::make_unique<MenuItem>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
                if (this->menuState == Normal && key.key != GK_A) return;
                if (!key.source) return;
                if (key.source->type != SDL_CONTROLLERBUTTONDOWN) return;
                if (this->menuState == Normal) {
                    this->menuState = AwaitBinding;
                } else if (this->menuState == AwaitBinding) {
                    Input::rebind(button, key.key);
                    this->menuState = Normal;
                }
            }));
}

void Menu::addKeyboardMenuItem(GameKey k, SDL_Keycode button) {
    auto bound = Input::toString(k);
    auto b = SDL_GetKeyName(button);
    items.push_back(
            std::make_unique<MenuItem>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
                if (this->menuState == Normal && key.key != GK_A) return;
                if (!key.source) return;
                if (key.source->type != SDL_KEYDOWN) return;
                if (this->menuState == Normal) {
                    this->menuState = AwaitBinding;
                } else if (this->menuState == AwaitBinding) {
                    Input::rebind(button, key.key);
                    this->menuState = Normal;
                }
            }));
}

void Menu::populateKeyboardMenu() {
    SDL_Keycode bound;

    bound = Input::boundKey(GK_UP);
    addKeyboardMenuItem(GK_UP, bound);

    bound = Input::boundKey(GK_DOWN);
    addKeyboardMenuItem(GK_DOWN, bound);

    bound = Input::boundKey(GK_LEFT);
    addKeyboardMenuItem(GK_LEFT, bound);

    bound = Input::boundKey(GK_RIGHT);
    addKeyboardMenuItem(GK_RIGHT, bound);

    bound = Input::boundKey(GK_A);
    addKeyboardMenuItem(GK_A, bound);

    bound = Input::boundKey(GK_B);
    addKeyboardMenuItem(GK_B, bound);

    bound = Input::boundKey(GK_X);
    addKeyboardMenuItem(GK_X, bound);

    bound = Input::boundKey(GK_Y);
    addKeyboardMenuItem(GK_Y, bound);

    bound = Input::boundKey(GK_L);
    addKeyboardMenuItem(GK_L, bound);

    bound = Input::boundKey(GK_R);
    addKeyboardMenuItem(GK_R, bound);

    bound = Input::boundKey(GK_START);
    addKeyboardMenuItem(GK_START, bound);

    bound = Input::boundKey(GK_SELECT);
    addKeyboardMenuItem(GK_SELECT, bound);
}

void Menu::populateGamepadMenu() {
    SDL_GameControllerButton bound;

    bound = Input::bound(GK_UP);
    addGamepadMenuItem(GK_UP, bound);

    bound = Input::bound(GK_DOWN);
    addGamepadMenuItem(GK_DOWN, bound);

    bound = Input::bound(GK_LEFT);
    addGamepadMenuItem(GK_LEFT, bound);

    bound = Input::bound(GK_RIGHT);
    addGamepadMenuItem(GK_RIGHT, bound);

    bound = Input::bound(GK_A);
    addGamepadMenuItem(GK_A, bound);

    bound = Input::bound(GK_B);
    addGamepadMenuItem(GK_B, bound);

    bound = Input::bound(GK_X);
    addGamepadMenuItem(GK_X, bound);

    bound = Input::bound(GK_Y);
    addGamepadMenuItem(GK_Y, bound);

    bound = Input::bound(GK_L);
    addGamepadMenuItem(GK_L, bound);

    bound = Input::bound(GK_R);
    addGamepadMenuItem(GK_R, bound);

    bound = Input::bound(GK_START);
    addGamepadMenuItem(GK_START, bound);

    bound = Input::bound(GK_SELECT);
    addGamepadMenuItem(GK_SELECT, bound);
}

MenuItem::MenuItem(std::string key) : key(key), cb(nullptr), canSelect(false) {}

MenuItem::MenuItem(std::string key, menu_Callback cb) : key(key), cb(cb), canSelect(true) {}

MenuItem::MenuItem(std::string key, std::string *value, menu_Callback cb) : key(key), value(value), cb(cb),
                                                                            canSelect(true) {}

MenuItem::MenuItem(const char *key, const char *value, menu_Callback cb) : key(key), value2(value), cb(cb),
                                                                           canSelect(true) {}


void MenuItem::call(GameKeyEvent &key) {
    if (this->cb) cb(key);
}

void MenuItem::render(bool selected) {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    if (this->value) {
        // Two rows: key and value from pointer
        s.x = s.x / 2;
        ImGui::Button(this->key.c_str(), s);
        ImGui::SameLine();
        ImGui::Button(this->value->c_str(), s);
    } else if (this->value2.size() > 0) {
        // Two rows: key and value from prepopulated string
        s.x = s.x / 2;
        ImGui::Button(this->key.c_str(), s);
        ImGui::SameLine();
        ImGui::Button(this->value2.c_str(), s);
    } else {
        // One row only
        ImGui::Button(this->key.c_str(), s);
    }

    if (selected) {
        ImGui::PopStyleColor(4);
    }
}
