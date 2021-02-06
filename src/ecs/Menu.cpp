#include "Menu.h"
#include <IMGUI/imgui.h>
#include <sstream>

#include "../config.h"
#include "../Rt.h"
#include "../St.h"

#define WINDOW_MARGIN 4

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

    ImGui::Button(ss.str().c_str(), getItemSize());

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

    ImGui::Button(ss.str().c_str(), getItemSize());

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }
}

void MenuOption::render() {
    auto s = getItemSize();
    ImGui::SetCursorPosX((configWindowWidth / 2) - (s.x / 2));

    if (this->selected) {
        ImGui::PushStyleColor(ImGuiCol_Button, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, white());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white());
        ImGui::PushStyleColor(ImGuiCol_Text, black());
    }

    ImGui::Button(this->text.c_str(), getItemSize());

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

    ImGui::Button(ss.str().c_str(), getItemSize());

    if (this->selected) {
        ImGui::PopStyleColor(4);
    }
}

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

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
            controllerSettings.render();
            break;
        default:
            break;
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

void MenuControllerSettings::key(GameKeyEvent &key) {

}

void MenuControllerSettings::render() {

}

void Menu::key(GameKeyEvent &key) {
    if (!key.valid) return;
    if (!key.state == GK_PUSHED) return;
    if (key.key == GK_B) {
        if (this->level.size() > 1) this->level.pop();
        this->selectedIndex = 0;
    } else if (key.key == GK_UP) {
        this->selectedIndex--;
        if (this->selectedIndex < 0) this->selectedIndex = currentMenuItems() - 1;
    } else if (key.key == GK_DOWN) {
        this->selectedIndex++;
        this->selectedIndex %= currentMenuItems();
    } else {
        switch (this->level.top()) {
            case Main:
                this->menu_Main.at(selectedIndex)->invoke(key);
                break;
            case Settings:
                this->menu_Settings.at(selectedIndex)->invoke(key);
                break;
            case AudioSettings:
                this->menu_AudioSettings.at(selectedIndex)->invoke(key);
                break;
            case VideoSettings:
                this->menu_VideoSettings.at(selectedIndex)->invoke(key);
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
        default:
            return 0;
    }
}