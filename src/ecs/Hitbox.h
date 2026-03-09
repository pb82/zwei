#ifndef ZWEI_HITBOX_H
#define ZWEI_HITBOX_H

#include <memory>

#include "Component.h"
#include "../alg/Force.h"
#include "../alg/Direction.h"

class Hitbox : public Component {
public:
    Hitbox(Entity &parent);

    Force force;

    const Entity *origin;

    float power;

    bool isRanged = false;

    Direction launchDirection;
};


#endif //ZWEI_HITBOX_H
