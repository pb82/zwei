#include "Acceleration.h"

#include "Entity.h"
#include "Transform.h"

Acceleration::Acceleration(Entity &parent, float maxSpeed, float angle)
        : Component(parent),
          trajectory(0, angle),
          maxSpeed(maxSpeed),
          speed(0),
          acceleration(acceleration) {}

void Acceleration::update(float dt) {
    auto transform = parent.getComponent<Transform>();

    // Remember the current position before mutating it
    last = transform->p;

    trajectory.radius = speed * (dt / 1000);
    trajectory.translate(&transform->p.x, &transform->p.y);

    forces.erase(
            std::remove_if(
                    forces.begin(),
                    forces.end(), [&](Force &f) {
                        f.update(100.0f, dt);
                        bool active = f.apply(100.0f, &transform->p.x, &transform->p.y);
                        if (!active) speed = 0;
                        return !active;
                    }),
            forces.end());

}

void Acceleration::applyForce(const Force &force) {
    if (forces.size() >= 5) return;
    forces.push_back(force);
}

void Acceleration::applyForce(float angle, float power) {
    if (forces.size() >= 5) return;
    forces.push_back(Force(angle, power));
}

void Acceleration::accelerate() {
    speed = maxSpeed;
}

void Acceleration::decelerate() {
    speed = 0;
}

void Acceleration::turn(float angle) {
    this->trajectory.angle = angle;
}

void Acceleration::reset(Position &target) {
    target = last;
}

float Acceleration::getAngle() {
    return this->trajectory.angle;
}

void Acceleration::setFacing(Direction d) {
    this->face = d;
}

Position &Acceleration::getPosition() {
    return this->last;
}

Direction Acceleration::getDirection() {
    if (face != NONE) return face;
    return this->trajectory.getDirection();
}

bool Acceleration::sameAngle(float other) {
    auto epsilon = std::numeric_limits<float>::epsilon();
    return std::abs(this->trajectory.angle - other) < epsilon;
}