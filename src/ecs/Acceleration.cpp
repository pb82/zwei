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

    forces.erase(std::remove_if(forces.begin(), forces.end(), [&](Force &f) {
        f.update(100.0f, dt);
        bool exhausted = f.apply(100.0f, &transform->p.x, &transform->p.y);
        if (!exhausted) {
            speed = 0;
        }
        return exhausted;
    }), forces.end());
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

void Acceleration::applyForce(float angle, float power) {
    forces.push_back(Force(angle, power));
}