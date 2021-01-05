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

    std::shared_ptr<Entity> getPlayer();

    Topology &getTopology() {
        return topology;
    }

    GameStateMachine state;

private:
    std::shared_ptr<Entity> player;

    Topology topology;

};


#endif
