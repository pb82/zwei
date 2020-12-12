#ifndef ZWEI_PROJECTILE_H
#define ZWEI_PROJECTILE_H

#include "Component.h"
#include "../alg/Force.h"

class Projectile : public Component {
public:
    Projectile(Entity& paret);

    Force force;

    float power;
};


#endif //ZWEI_PROJECTILE_H
