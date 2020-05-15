#ifndef ZWEI_ACCELERATION_H
#define ZWEI_ACCELERATION_H

#include <vector>

#include "../alg/Vector.h"
#include "../alg/Force.h"

#include "Component.h"

class Acceleration : public Component {
public:
    Acceleration(Entity &parent, float maxSpeed, float acceleration, float angle);

    // Acceleration in tiles/s²
    float acceleration = 0.0f;

    // Current speed in tiles/s
    float speed = 0.0f;

    // Maximum speed at the moment. Gets set to zero in order to
    // stop movement
    float maxSpeed;

    // The value the max speed is reset to (the actual
    // max speed)
    float resetSpeed;

    void update(float dt) override;

    void accelerate(float dt);

    void decelerate(float dt);

    void applyForce(float angle, float power);

    Vector trajectory;

private:

    std::vector<Force> forces;

};

#endif
