#ifndef ZWEI_CTX_H
#define ZWEI_CTX_H

#include <memory>
#include <stack>

#include "ecs/Entity.h"
#include "alg/Path.h"

enum GameState {
    Game,
    GameOver,
    MainMenu,
};

struct GameStateMachine {
    GameStateMachine();

    void toggleMenu();

    void pushState(GameState state);

    GameState currentState();

private:

    std::stack<GameState> current;
};

// Shared between all entities
class Ctx {
public:
    void setPlayer(std::shared_ptr<Entity> player);

    void setMenu(std::shared_ptr<Entity> menu);

    std::shared_ptr<Entity> getPlayer();

    std::shared_ptr<Entity> getMenu();

    Topology &getTopology() {
        return topology;
    }

    GameStateMachine state;

private:
    std::shared_ptr<Entity> player = nullptr;

    std::shared_ptr<Entity> menu = nullptr;

    Topology topology;

};


#endif
