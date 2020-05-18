#ifndef ZWEI_MANAGER_H
#define ZWEI_MANAGER_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "Entity.h"

class Collider;

class Manager {
public:
    static Manager &instance() {
        static Manager instance;
        return instance;
    }

    Manager(Manager const &) = delete;

    void operator=(Manager const &) = delete;

    std::shared_ptr<Entity> addEntity(LayerType layer);

    void getColliders(std::vector<std::shared_ptr<Collider>> &target);

    void update(float dt);

    void render(LayerType layer);

    void key(GameKeyEvent &key);

private:
    Manager() {}

    std::unordered_map<LayerType, std::vector<std::shared_ptr<Entity>>> entities;
};

#endif
