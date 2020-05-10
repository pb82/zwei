#include "Acceleration.h"

#include "Entity.h"
#include "Transform.h"

Acceleration::Acceleration(Entity &parent, float maxSpeed, float acceleration, float angle)
        : Component(parent),
          trajectory(0, angle),
          maxSpeed(maxSpeed),
          acceleration(acceleration) {}

void Acceleration::update(float dt) {
    auto transform = parent.getComponent<Transform>();
    trajectory.radius = speed * (dt / 1000);
    trajectory.translate(&transform->p.x, &transform->p.y);
}

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