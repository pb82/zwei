#ifndef ZWEI_ACCELERATION_H
#define ZWEI_ACCELERATION_H

#include <vector>

#include "../alg/Vector.h"
#include "../alg/Force.h"

#include "Component.h"

class Acceleration : public Component {
public:
    Acceleration(Entity &parent, float maxSpeed, float acceleration, float angle);

    float speed = 0.0f;

    float maxSpeed;

    void update(float dt) override;

    void accelerate(float dt);

    void decelerate(float dt);

    void applyForce(float angle, float power);

    Vector trajectory;

private:

    std::vector<Force> forces;

    float acceleration;

};

#endif
