#ifndef ZWEI_MANAGER_H
#define ZWEI_MANAGER_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <queue>

#include "Entity.h"

class Collider;

struct QueueEntity {
    std::shared_ptr<Entity> entity;
    LayerType layer;
};

class Manager {
public:
    static Manager &instance() {
        static Manager instance;
        return instance;
    }

    Manager(Manager const &) = delete;

    void operator=(Manager const &) = delete;

    std::shared_ptr<Entity> addEntity(LayerType layer);

    void enqueue(std::shared_ptr<Entity> entity, LayerType layer);

    // add new entities and garbage collect disabled ones
    void collect();

    void getColliders(std::vector<std::shared_ptr<Collider>> &target);

    void update(float dt);

    void render(LayerType layer);

    void key(GameKeyEvent &key);

private:
    Manager() {}

    std::unordered_map<LayerType, std::vector<std::shared_ptr<Entity>>> entities;

    std::queue<QueueEntity> pendingEntities;
};

#endif
