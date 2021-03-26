#include "Menu.h"
#include <IMGUI/imgui.h>
#include <sstream>

#include "../config.h"
#include "../Rt.h"
#include "../scn/Start.h"

#define WINDOW_MARGIN 0

// WARNING !!!
// This is possibly the worst implementation of a menu that has ever been written

static void getWinSize(ImVec2 &s) {
    s.x = configWindowWidth - WINDOW_MARGIN;
    s.y = configWindowHeight - WINDOW_MARGIN;
}

static void getWinPos(ImVec2 &p) {
    p.x = WINDOW_MARGIN / 2;
    p.y = WINDOW_MARGIN / 2;
}

static ImVec2 getItemSize() {
    ImVec2 size;
    size.x = configWindowWidth / 2;
    size.y = 32;
    return size;
}

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

static int getWinFlags() {
    return ImGuiWindowFlags_NoDecoration
           | ImGuiWindowFlags_NoResize
           | ImGuiWindowFlags_NoMove
           | ImGuiWindowFlags_NoCollapse;
}

MenuOption::MenuOption(const char *text, menu_Callback cb) : MenuAction(text, cb) {}

MenuMusicVolume::MenuMusicVolume(bool music, menu_Callback cb) : MenuAction("", cb), music(music) {}

MenuScreenResolution::MenuScreenResolution(menu_Callback cb) : MenuAction("", cb) {}

MenuFps::MenuFps(menu_Callback cb) : MenuAction("", cb) {}

MenuKeyBinding::MenuKeyBinding(const char *action, const char *bound, menu_Callback cb) : MenuAction(action, cb),
                                                                                          bound(bound) {}

void MenuFps::render() {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    std::stringstream ss;
    ss << "Target fps: ";
    ss << St::instance().getFps();

    if (this->selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    ImGui::Button(ss.str().c_str(), s);

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }
}

void MenuScreenResolution::render() {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    std::stringstream ss;
    ss << "Window size: ";
    ss << St::instance().getWindowSize().w;
    ss << "x";
    ss << St::instance().getWindowSize().h;

    if (this->selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    ImGui::Button(ss.str().c_str(), s);

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }
}

void MenuOption::render() {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    if (this->selected) {
        if (!enabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, lightgrey());
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, lightgrey());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, lightgrey());
            ImGui::PushStyleColor(ImGuiCol_Text, grey());
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, white());
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
            ImGui::PushStyleColor(ImGuiCol_Text, black());
        }
    }

    if (!this->enabled) {
        ImGui::PushStyleColor(ImGuiCol_Text, grey());
    }

    ImGui::Button(this->text.c_str(), s);

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }

    if (!enabled) {
        ImGui::PopStyleColor(1);
    }
}

void MenuKeyBinding::render() {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));
    s.x = s.x / 2;

    if (this->selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    ImGui::Button(this->bound.c_str(), s);
    ImGui::SameLine();
    ImGui::Button(this->text.c_str(), s);

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }

}

void MenuMusicVolume::render() {
    std::stringstream ss;

    if (this->music) {
        ss << "Music Volume: ";
        ss << St::instance().getMusicVolume();
    } else {
        ss << "Effects Volume: ";
        ss << St::instance().getEffectsVolume();
    }
    ss << "%";

    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    if (this->selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    ImGui::Button(ss.str().c_str(), s);

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }
}

void Menu::newGameEnabled(bool enabled) {
    menu_Main.at(0)->enabled = enabled;
}

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

    menu_Main.push_back(std::make_unique<MenuOption>("New Game", [](GameKeyEvent &key) {
        if (key.key == GK_A) {
            RT_Context.setActiveScene<Start>();
        }
    }));
    menu_Main.at(0)->enabled = false;

    menu_Main.push_back(std::make_unique<MenuOption>("Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Settings);
        this->selectedIndex = 0;
    }));

    menu_Main.push_back(std::make_unique<MenuOption>("Quit", [](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        St::instance().serialize();
        RT_Running = false;
    }));
    menu_Settings.push_back(std::make_unique<MenuOption>("Audio Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(AudioSettings);
        this->selectedIndex = 0;
    }));
    menu_Settings.push_back(std::make_unique<MenuOption>("Video Settings", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(VideoSettings);
        this->selectedIndex = 0;
    }));
    menu_Settings.push_back(std::make_unique<MenuOption>("Controls", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Controls);
        this->selectedIndex = 0;
    }));
    menu_Settings.push_back(std::make_unique<MenuOption>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));
    menu_AudioSettings.push_back(std::make_unique<MenuMusicVolume>(true, [](GameKeyEvent &key) {
        if (key.key == GK_LEFT) {
            St::instance().decMusicVolume();
        } else if (key.key == GK_RIGHT) {
            St::instance().incMusicVolume();
        }
    }));
    menu_AudioSettings.push_back(std::make_unique<MenuMusicVolume>(false, [](GameKeyEvent &key) {
        if (key.key == GK_LEFT) {
            St::instance().decEffectsVolume();
        } else if (key.key == GK_RIGHT) {
            St::instance().incEffectsVolume();
        }
    }));
    menu_AudioSettings.push_back(std::make_unique<MenuOption>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));
    menu_VideoSettings.push_back(std::make_unique<MenuScreenResolution>([](GameKeyEvent &key) {
        if (key.key == GK_LEFT) {
            St::instance().decWindowSize();
        } else if (key.key == GK_RIGHT) {
            St::instance().incWindowSize();
        }
    }));
    menu_VideoSettings.push_back(std::make_unique<MenuFps>([](GameKeyEvent &key) {
        if (key.key == GK_LEFT) {
            St::instance().decFps();
        } else if (key.key == GK_RIGHT) {
            St::instance().incFps();
        }
    }));
    menu_VideoSettings.push_back(std::make_unique<MenuOption>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));

    menu_ControllerSettings.push_back(std::make_unique<MenuOption>("Keyboard", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Keyboard);
        this->selectedIndex = 0;
    }));

    menu_ControllerSettings.push_back(std::make_unique<MenuOption>("Gamepad", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.push(Gamepad);
        this->selectedIndex = 0;
    }));

    menu_ControllerSettings.push_back(std::make_unique<MenuOption>("Back", [this](GameKeyEvent &key) {
        if (key.key != GK_A) return;
        this->level.pop();
        this->selectedIndex = 0;
    }));

    populateGamepadMenu();
    populateKeyboardMenu();
}

void Menu::addGamepadMenuItem(GameKey k, SDL_GameControllerButton button) {
    auto bound = Input::toString(k);
    auto b = SDL_GameControllerGetStringForButton(button);
    menu_GamecontrollerSettings.push_back(
            std::make_unique<MenuKeyBinding>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
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
    menu_KeyboardSettings.push_back(
            std::make_unique<MenuKeyBinding>(bound.c_str(), b, [this, button](GameKeyEvent &key) {
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
    menu_KeyboardSettings.clear();
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
    menu_GamecontrollerSettings.clear();
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

void Menu::render() {
    ImVec2 size;
    getWinSize(size);

    ImVec2 position;
    getWinPos(position);

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Menu", nullptr, getWinFlags());

    switch (this->level.top()) {
        case Main:
            renderMenu(menu_Main);
            break;
        case Settings:
            renderMenu(menu_Settings);
            break;
        case AudioSettings:
            renderMenu(menu_AudioSettings);
            break;
        case VideoSettings:
            renderMenu(menu_VideoSettings);
            break;
        case Controls:
            renderMenu(menu_ControllerSettings);
            break;
        case Gamepad:
            renderMenu(menu_GamecontrollerSettings);
            break;
        case Keyboard:
            renderMenu(menu_KeyboardSettings);
            break;
        default:
            break;
    }

    if (this->menuState == AwaitBinding) {
        ImGui::Text("Enter new binding...");
    }

    ImGui::End();
}

void Menu::renderMenu(std::vector<std::unique_ptr<MenuAction>> &menu) {
    for (int i = 0; i < menu.size(); i++) {
        menu.at(i)->render();
        if (i == selectedIndex) menu.at(i)->select();
        else menu.at(i)->unselect();
    }
}

void Menu::key(GameKeyEvent &key) {
    if (!key.valid) return;
    if (key.state != GK_PUSHED) return;
    if (key.key == GK_B && menuState != AwaitBinding) {
        if (this->level.size() > 1) this->level.pop();
        this->selectedIndex = 0;
    } else if (key.key == GK_UP && menuState != AwaitBinding) {
        this->selectedIndex--;
        if (this->selectedIndex < 0) this->selectedIndex = currentMenuItems() - 1;
    } else if (key.key == GK_DOWN && menuState != AwaitBinding) {
        this->selectedIndex++;
        this->selectedIndex %= currentMenuItems();
    } else {
        switch (this->level.top()) {
            case Main:
                if (this->menu_Main.at(selectedIndex)->enabled) this->menu_Main.at(selectedIndex)->invoke(key);
                break;
            case Settings:
                if (this->menu_Settings.at(selectedIndex)->enabled) this->menu_Settings.at(selectedIndex)->invoke(key);
                break;
            case AudioSettings:
                this->menu_AudioSettings.at(selectedIndex)->invoke(key);
                break;
            case VideoSettings:
                this->menu_VideoSettings.at(selectedIndex)->invoke(key);
                break;
            case Controls:
                this->menu_ControllerSettings.at(selectedIndex)->invoke(key);
                break;
            case Gamepad:
                this->menu_GamecontrollerSettings.at(selectedIndex)->invoke(key);
                if (this->menuState == Normal) populateGamepadMenu();
                break;
            case Keyboard:
                this->menu_KeyboardSettings.at(selectedIndex)->invoke(key);
                if (this->menuState == Normal) populateKeyboardMenu();
                break;
        }
    }
}

int Menu::currentMenuItems() {
    switch (this->level.top()) {
        case Main:
            return menu_Main.size();
        case Settings:
            return menu_Settings.size();
        case AudioSettings:
            return menu_AudioSettings.size();
        case VideoSettings:
            return menu_VideoSettings.size();
        case Controls:
            return menu_ControllerSettings.size();
        case Gamepad:
            return Input::controllerMapping.size();
        case Keyboard:
            return Input::keyboardMapping.size();
        default:
            return 0;
    }
}

void Menu::reset(int selectedIndex) {
    while (!this->level.empty()) this->level.pop();
    this->level.push(Main);
    this->selectedIndex = selectedIndex;
}