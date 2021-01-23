#ifndef ZWEI_PROJECTILE_H
#define ZWEI_PROJECTILE_H

#include <memory>

#include "Component.h"
#include "../alg/Force.h"
#include "../alg/Direction.h"

class Projectile : public Component {
public:
    Projectile(Entity &parent);

    Force force;

    const Entity *origin;

    float power;

    bool isProjectile = false;

    Direction launchDirection;
};


#endif //ZWEI_PROJECTILE_H
