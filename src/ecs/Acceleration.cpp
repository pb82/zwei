#include "Acceleration.h"

#include "Entity.h"
#include "Transform.h"

Acceleration::Acceleration(Entity &parent, float maxSpeed, float angle)
        : Component(parent),
          trajectory(0, angle),
          maxSpeed(maxSpeed),
          speed(0),
          acceleration(0.0f) {}

void Acceleration::update(float dt) {
    auto transform = parent.getComponent<Transform>();

    // Remember the current position before mutating it
    last = transform->p;

    trajectory.radius = speed * (dt / 1000);
    trajectory.translate(&transform->p.x, &transform->p.y);

    // Map bounds check
    int mapW, mapH;
    Rt_Map.getSize(FLOOR, &mapW, &mapH);
    if (transform->p.x <= 0 || transform->p.x >= mapW) {
        reset(transform->p);
        return;
    }

    if (transform->p.y <= 0 || transform->p.y >= mapH) {
        reset(transform->p);
        return;
    }

    forces.erase(
            std::remove_if(
                    forces.begin(),
                    forces.end(), [&](Force &f) {
                        f.update(dt);
                        bool active = f.apply(&transform->p.x, &transform->p.y);
                        if (!active) speed = 0;
                        return !active;
                    }),
            forces.end());

}

bool Acceleration::hasForces() {
    return !this->forces.empty();
}

void Acceleration::applyForce(const Force &force) {
    if (forces.size() >= 5) return;
    forces.push_back(force);
}

void Acceleration::applyForce(float angle, float power, float decay, float weight) {
    if (forces.size() >= 5) return;
    forces.emplace_back(angle, power, decay, weight);
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

void Acceleration::setSpeed(float maxSpeed) {
    this->maxSpeed = maxSpeed;
}

void Acceleration::setDirection(Direction d) {
    this->trajectory.setDirection(d);
}

bool Acceleration::sameAngle(float other) {
    auto epsilon = std::numeric_limits<float>::epsilon();
    return std::abs(this->trajectory.angle - other) < epsilon;
}