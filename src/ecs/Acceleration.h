#ifndef ZWEI_ACCELERATION_H
#define ZWEI_ACCELERATION_H

#include "Component.h"

class Acceleration : public Component {
public:
    Acceleration(Entity &parent, float maxSpeed, float acceleration);

    float speed = 0.0f;

    void accelerate(float dt);

    void decelerate(float dt);

private:

    float acceleration;

    float maxSpeed;
};

#endif
