#ifndef ZWEI_MENU_H
#define ZWEI_MENU_H

#include <stack>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <string>

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
    AreYouSure,
    Controls,
    Keyboard,
    Gamepad
};

enum MenuItemType {
    ItemNewGame,
    ItemAudioSettings,
    ItemMusicVolume,
    ItemEffectsVolume,
    ItemVideoSettings,
    ItemFps,
    ItemWindowSize,
    ItemQuit,
    ItemAreYouSure,
    ItemYes,
    ItemNo,
    ItemBack,
};

typedef std::function<void(GameKeyEvent &key)> menu_Callback;

class MenuItem {
public:

    MenuItem(std::string text);

    MenuItem(std::string key, menu_Callback cb);

    MenuItem(std::string key, std::string *value, menu_Callback cb);

    void call(GameKeyEvent &key);

    void render(bool selected = false);

    bool canSelect = true;

private:

    std::string key;

    std::string *value = nullptr;

    menu_Callback cb;

};

class Menu : public Component {
public:
    Menu(Entity &parent);

    void render(uint8_t) override;

    void key(GameKeyEvent &key) override;

private:

    std::string currentFps;

    std::string windowSize;

    std::string musicVolume;

    std::string effectsVolume;

    std::vector<std::shared_ptr<MenuItem>> items;

    std::map<MenuItemType, std::shared_ptr<MenuItem>> allItems;

    void buildMenu();

    // On game start, no map has been loaded yet
    void buildStartMenu();

    void up();

    void down();

    int selectedIndex = 0;

    MenuState menuState = Normal;

    std::stack<GameMenu> level;
};


#endif //ZWEI_MENU_H
