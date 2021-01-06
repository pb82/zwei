#ifndef ZWEI_MENU_H
#define ZWEI_MENU_H

#include <stack>

#include "Component.h"

enum GameMenu {
    Main,
};

class Menu : public Component {
public:
    Menu(Entity &parent);

    void render() override;

private:

    std::stack<GameMenu> level;

    void renderMainMenu();

};


#endif //ZWEI_MENU_H
