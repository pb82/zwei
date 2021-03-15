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

    template<typename T>
    void addScene() {
        std::shared_ptr<Scene> scene = std::make_shared<T>();
        auto type = std::type_index(typeid(T));
        scenes.template emplace(type, scene);
    }

    template<typename T>
    void setActiveScene() {
        if (activeScene) {
            activeScene->exit();
            activeScene = nullptr;
        }
        auto type = std::type_index(typeid(T));
        activeScene = scenes.at(type);
        activeScene->init();
    }

    std::shared_ptr<Entity> getPlayer();

    std::shared_ptr<Entity> getMenu();

    Topology &getTopology() {
        return topology;
    }

    GameStateMachine state;

private:
    std::shared_ptr<Entity> player = nullptr;

    std::shared_ptr<Entity> menu = nullptr;

    std::unordered_map<std::type_index, std::shared_ptr<Scene>> scenes;

    std::shared_ptr<Scene> activeScene = nullptr;

    Topology topology;

};


#endif
