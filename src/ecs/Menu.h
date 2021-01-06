#ifndef ZWEI_MENU_H
#define ZWEI_MENU_H

#include <stack>
#include <memory>
#include <vector>
#include <functional>

#include "Component.h"

enum GameMenu {
    Main,
    Settings,
};

typedef std::function<void()> menu_Callback;

class MenuAction {
public:
    MenuAction(const char *text) : text(text) {}

    virtual void invoke(GameKeyEvent &key) = 0;

    virtual void render() = 0;

    void select() { this->selected = true; }

    void unselect() { this->selected = false; }

protected:

    std::string text;

    bool selected = false;

};

class MenuOption : public MenuAction {
public:
    MenuOption(const char *text, menu_Callback cb);

    void invoke(GameKeyEvent &key) override;

    void render() override;

private:

    menu_Callback cb;

};

class Menu : public Component {
public:
    Menu(Entity &parent);

    void render() override;

    void key(GameKeyEvent &key) override;

private:

    void renderMenu(std::vector<std::unique_ptr<MenuAction>> &menu);

    int currentMenuItems();

    std::stack<GameMenu> level;

    std::vector<std::unique_ptr<MenuAction>> menu_Main;
    std::vector<std::unique_ptr<MenuAction>> menu_Settings;


    int selectedIndex = 0;

};


#endif //ZWEI_MENU_H
