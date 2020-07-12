#ifndef ZWEI_CTX_H
#define ZWEI_CTX_H

#include <memory>

#include "ecs/Entity.h"
#include "alg/Path.h"

// Shared between all entities
class Ctx {
public:
    void setPlayer(std::shared_ptr<Entity> player);

    std::shared_ptr<Entity> getPlayer();

    Topology &getTopology() {
        return topology;
    }

private:
    std::shared_ptr<Entity> player;

    Topology topology;
};


#endif
