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

    bool toggleMenu();

    void popState();

    void pushState(GameState state);

    GameState currentState();

private:

    std::stack<GameState> current;
};

// Shared between all entities
class Ctx {
public:
    Ctx();

    ~Ctx() {
        if (saved) delete saved;
    }

    void setPlayer(std::shared_ptr<Entity> player);

    void setMenu(std::shared_ptr<Entity> menu);

    void save(float x, float y);

    bool savegameExists();

    void load(float *x, float *y);

    void setActiveScene(SceneType s, JSON::Value &resumedEnemies = JSON::null) {
        if (scenes.find(s) == scenes.end()) return;
        auto &scene = scenes.at(s);

        if (activeScene) {
            activeScene->exit();
            activeScene.reset();
        }

        activeScene = scene;
        activeScene->init();
        activeScene->addEnemies(resumedEnemies);
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

    std::unordered_map<SceneType, std::shared_ptr<Scene>> scenes;

    Topology topology;

    Mem memory;

    bool *saved = nullptr;
};


#endif
