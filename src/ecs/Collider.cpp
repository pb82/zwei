#include "Collider.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Ai.h"
#include "Attack.h"
#include "SelfDestruct.h"
#include "Bloat.h"
#include "Collectable.h"
#include "Controller.h"

Collider::Collider(Entity &parent, std::shared_ptr<Transform> tracked, ColliderTag tag,
                   Padding p) :
        Component(parent),
        tracked(tracked),
        tag(tag),
        padding(p) {
    updateBoundingBox();
}

void Collider::updateBoundingBox() {
    RT_Camera.project(boundingBox, tracked->p.x, tracked->p.y, padding);
}

void Collider::update(float dt) {
    updateBoundingBox();
}

void Collider::setPadding(Padding p) {
    this->padding = p;
}

// Stop a mobile target
void Collider::stop(const Collider &c) {
    if (c.parent.hasComponent<Acceleration>()) {
        auto acceleration = c.parent.getComponent<Acceleration>();
        acceleration->decelerate();
        acceleration->reset(c.tracked->p);
    }
}

void Collider::stop() {
    if (this->parent.hasComponent<Acceleration>()) {
        auto acceleration = this->parent.getComponent<Acceleration>();
        acceleration->decelerate();
        acceleration->reset(this->tracked->p);
    }
}

void Collider::pause(float duration) {
    if (this->parent.hasComponent<Ai>()) {
        auto ai = this->parent.getComponent<Ai>();
        ai->pause(duration);
    }
}

void Collider::kick(std::shared_ptr<Collider> other) {
    auto a1 = this->parent.getComponent<Transform>();
    auto a2 = other->parent.getComponent<Acceleration>();

    float angle = a1->p.angle(a2->getPosition());

    a2->applyForce(angle, 10);
}

bool Collider::defend(std::shared_ptr<Collider> projectile) {
    if (projectile->parent.hasComponent<Projectile>()) {
        auto p = projectile->parent.getComponent<Projectile>();
        if (p->origin == &this->parent) {
            // No need to defend against your own projectiles
            return false;
        }

        if (parent.hasComponent<Attack>()) {
            auto a = parent.getComponent<Attack>();
            a->defend(p);
        }
    }
    return true;
}

void Collider::disable() {
    this->parent.disable();
}

void Collider::collect() {
    if (this->parent.hasComponent<Collectable>()) {
        auto c = this->parent.getComponent<Collectable>();
        c->collect();
    }
};
