#ifndef ZWEI_ACCELERATION_H
#define ZWEI_ACCELERATION_H

#include <vector>

#include "../alg/Vector.h"
#include "../alg/Force.h"
#include "../alg/Position.h"

#include "Component.h"

class Acceleration : public Component {
public:
    Acceleration(Entity &parent, float maxSpeed, float angle);

    // Acceleration in tiles/s²
    float acceleration = 0.0f;

    // Current speed in tiles/s
    float speed = 0.0f;

    // Maximum speed at the moment. Gets set to zero in order to
    // stop movement
    float maxSpeed;

    void update(float dt) override;

    void accelerate();

    void decelerate();

    void applyForce(float angle, float power);

    void turn(float angle);

    void reset(Position &target);

    float getAngle();

    Position& getPosition();

    bool sameAngle(float other);

    Direction getDirection();

private:

    Vector trajectory;

    Position last;

    std::vector<Force> forces;

};

#endif
