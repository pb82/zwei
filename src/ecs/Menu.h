#ifndef ZWEI_MENU_H
#define ZWEI_MENU_H

#include <stack>
#include <memory>
#include <vector>

#include "Component.h"

enum GameMenu {
    Main,
};

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
    MenuOption(const char *text);

    void invoke(GameKeyEvent &key) override;

    void render() override;
};

class Menu : public Component {
public:
    Menu(Entity &parent);

    void render() override;

    void key(GameKeyEvent &key) override;

private:

    std::stack<GameMenu> level;

    void renderMainMenu();

    std::vector<std::unique_ptr<MenuAction>> mainMenu;

    int selectedIndex = 0;

};


#endif //ZWEI_MENU_H
