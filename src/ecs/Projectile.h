#ifndef ZWEI_PROJECTILE_H
#define ZWEI_PROJECTILE_H

#include <memory>

#include "Component.h"
#include "../alg/Force.h"

class Projectile : public Component {
public:
    Projectile(Entity &parent);

    Force force;

    const Entity* origin;

    float power;
};


#endif //ZWEI_PROJECTILE_H
