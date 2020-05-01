#ifndef ZWEI_MANAGER_H
#define ZWEI_MANAGER_H

#include <vector>
#include <memory>

#include "Entity.h"

class Manager {
public:
    static Manager &instance() {
        static Manager instance;
        return instance;
    }

    Manager(Manager const &) = delete;

    void operator=(Manager const &) = delete;

    std::shared_ptr<Entity> addEntity();

    void update(float dt);

    void render();

private:
    Manager() {}

    std::vector<std::shared_ptr<Entity>> entities;
};

#endif
