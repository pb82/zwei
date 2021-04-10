#ifndef ZWEI_CTX_H
#define ZWEI_CTX_H

#include <memory>
#include <stack>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "Scene.h"
#include "ecs/Entity.h"
#include "alg/Path.h"
#include "Mem.h"

enum GameState {
    StateGame,
    StateGameOver,
    StateStart,
    StateMainMenu,
    StateSaving,
    StateLoading
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

    void setActiveScene(std::shared_ptr<Scene> s) {
        if (activeScene) {
            activeScene->exit();
            activeScene.reset();
        }
        activeScene = s;
        activeScene->init();
    }

    std::shared_ptr<Entity> getPlayer();

    std::shared_ptr<Entity> getMenu();

    Topology &getTopology() {
        return topology;
    }

    Mem &getMemory() {
        return memory;
    }

    GameStateMachine state;

private:
    std::shared_ptr<Entity> player = nullptr;

    std::shared_ptr<Entity> menu = nullptr;

    std::shared_ptr<Scene> activeScene = nullptr;

    Topology topology;

    Mem memory;

};


#endif
