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

Menu::Menu(Entity &parent) : Component(parent) {
    this->level.push(Main);
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

}