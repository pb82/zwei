#ifndef ZWEI_ACCELERATION_H
#define ZWEI_ACCELERATION_H

#include "../alg/Vector.h"

#include "Component.h"

class Acceleration : public Component {
public:
    Acceleration(Entity &parent, float maxSpeed, float acceleration, float angle);

    float speed = 0.0f;

    void update(float dt) override;

    void accelerate(float dt);

    void decelerate(float dt);

    Vector trajectory;

private:

    float acceleration;

    float maxSpeed;
};

#endif
