#include "Menu.h"
#include <IMGUI/imgui.h>

#include "../config.h"
#include "../Rt.h"

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
    size.x = configWindowWidth / 3;
    size.y = 24;
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

MenuOption::MenuOption(const char *text, menu_Callback cb) : MenuAction(text), cb(cb) {}

void MenuOption::invoke(GameKeyEvent &key) {
    this->cb();
}

void MenuOption::render() {
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

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

    menu_Main.push_back(std::make_unique<MenuOption>("Settings", [this]() {
        this->level.push(Settings);
        this->selectedIndex = 0;
    }));
    menu_Main.push_back(std::make_unique<MenuOption>("Quit", []() {
        RT_Running = false;
    }));
    menu_Settings.push_back(std::make_unique<MenuOption>("Audio Settings", []() {}));
    menu_Settings.push_back(std::make_unique<MenuOption>("Video Settings", []() {}));
    menu_Settings.push_back(std::make_unique<MenuOption>("Back", [this]() {
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

    ImGui::Columns(3);
    ImGui::NextColumn();

    switch (this->level.top()) {
        case Main:
            renderMenu(menu_Main);
            break;
        case Settings:
            renderMenu(menu_Settings);
        default:
            break;
    }

    ImGui::NextColumn();
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
        }
    }
}

int Menu::currentMenuItems() {
    switch (this->level.top()) {
        case Main:
            return menu_Main.size();
        case Settings:
            return menu_Settings.size();
        default:
            return 0;
    }
}