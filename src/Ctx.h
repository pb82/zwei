#ifndef ZWEI_CTX_H
#define ZWEI_CTX_H

#include <memory>

#include "ecs/Entity.h"

// Shared between all entities
class Ctx {
public:
    void setPlayer(std::shared_ptr<Entity> player);

    std::shared_ptr<Entity> getPlayer();

private:
    std::shared_ptr<Entity> player;
};


#endif
