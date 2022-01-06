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

    void applyForce(float angle, float power, float decay = 100.0f, float weight = 100.0f);

    void applyForce(const Force &force);

    void turn(float angle);

    void reset(Position &target);

    void setFacing(Direction d);

    float getAngle();

    bool hasForces();

    Position &getPosition();

    bool sameAngle(float other);

    Direction getDirection();

    void setDirection(Direction d);

    void setSpeed(float maxSpeed);

private:

    Direction face = NONE;

    Vector trajectory;

    Position last;

    std::vector<Force> forces;

};

#endif
