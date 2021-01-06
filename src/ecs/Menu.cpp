#include "Menu.h"
#include <IMGUI/imgui.h>

#include "../config.h"

#define WINDOW_MARGIN 4

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

MenuOption::MenuOption(const char *text) : MenuAction(text) {}

void MenuOption::invoke(GameKeyEvent &key) {}

void MenuOption::render() {}

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);

    mainMenu.push_back(std::make_unique<MenuOption>("Test"));
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
            renderMainMenu();
            break;
        default:
            break;
    }
    ImGui::End();
}

void Menu::renderMainMenu() {
    for (int i = 0; i < mainMenu.size(); i++) {
        mainMenu.at(i)->render();
        if (i == selectedIndex) mainMenu.at(i)->select();
        else mainMenu.at(i)->unselect();
    }
}

void Menu::key(GameKeyEvent &key) {

}