#ifndef ZWEI_MENU_H
#define ZWEI_MENU_H

#include <stack>
#include <memory>
#include <vector>
#include <functional>

#include "Component.h"

enum MenuState {
    Normal,
    AwaitBinding,
};

enum GameMenu {
    Main,
    Settings,
    AudioSettings,
    VideoSettings,
    Controls,
    Keyboard,
    Gamepad
};

typedef std::function<void(GameKeyEvent &key)> menu_Callback;

class MenuAction {
public:
    MenuAction(std::string text, menu_Callback cb) : text(text), cb(cb) {}

    virtual void invoke(GameKeyEvent &key) { this->cb(key); }

    virtual void render() = 0;

    void select() { this->selected = true; }

    void unselect() { this->selected = false; }

protected:

    std::string text;

    menu_Callback cb;

    bool selected = false;

};

class MenuOption : public MenuAction {
public:
    MenuOption(const char *text, menu_Callback cb);

    void render() override;

};

class MenuMusicVolume : public MenuAction {
public:
    MenuMusicVolume(bool music, menu_Callback cb);

    void render() override;

private:

    bool music;

};

class MenuScreenResolution : public MenuAction {
public:
    MenuScreenResolution(menu_Callback cb);

    void render() override;
};

class MenuFps : public MenuAction {
public:
    MenuFps(menu_Callback cb);

    void render() override;
};

class MenuControllerSettings {
public:

    void key(GameKeyEvent &key);

    void render();

};

class Menu : public Component {
public:
    Menu(Entity &parent);

    void render() override;

    void key(GameKeyEvent &key) override;

private:

    void renderMenu(std::vector<std::unique_ptr<MenuAction>> &menu);

    void renderControllerSettings();

    int currentMenuItems();

    void populateGamepadMenu();

    std::stack<GameMenu> level;

    MenuControllerSettings controllerSettings;

    std::vector<std::unique_ptr<MenuAction>> menu_Main;
    std::vector<std::unique_ptr<MenuAction>> menu_Settings;
    std::vector<std::unique_ptr<MenuAction>> menu_AudioSettings;
    std::vector<std::unique_ptr<MenuAction>> menu_VideoSettings;
    std::vector<std::unique_ptr<MenuAction>> menu_ControllerSettings;
    std::vector<std::unique_ptr<MenuAction>> menu_GamecontrollerSettings;


    int selectedIndex = 0;

    MenuState menuState = Normal;
};


#endif //ZWEI_MENU_H
