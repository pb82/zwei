#include "Acceleration.h"

Acceleration::Acceleration(Entity &parent, float maxSpeed, float acceleration)
        : Component(parent),
          maxSpeed(maxSpeed),
          acceleration(acceleration) {}

void Acceleration::accelerate(float dt) {
    if (speed >= maxSpeed) {
        speed = maxSpeed;
        return;
    }
    speed += acceleration * (dt / 1000);
}

void Acceleration::decelerate(float dt) {
    if (speed <= 0) {
        speed = 0.0f;
        return;
    }
    speed -= acceleration * (dt / 1000);
}