#include "Collider.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Ai.h"

Collider::Collider(Entity &parent, std::shared_ptr<Transform> tracked, ColliderTag tag,
                   float dx, float dy) :
        Component(parent),
        tracked(tracked),
        tag(tag),
        dx(dx),
        dy(dy) {
    updateBoundingBox();
}

void Collider::updateBoundingBox() {
    RT_Camera.project(boundingBox, tracked->p.x, tracked->p.y, dx, dy);
}

void Collider::update(float dt) {
    updateBoundingBox();
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

void Collider::turn() {
    if (this->parent.hasComponent<Acceleration>()) {
        auto acceleration = this->parent.getComponent<Acceleration>();
        acceleration->turn(acceleration->getAngle() - VM_100_PI);
    }
}

void Collider::kick(std::shared_ptr<Collider> other) {
    auto a1 = this->parent.getComponent<Acceleration>();
    auto a2 = other->parent.getComponent<Acceleration>();

    float angle = a1->getPosition().angle(a2->getPosition());

    a2->applyForce(angle, 10);
}

// Notify an AI-enabled entity that it collided with something
void Collider::notify(std::shared_ptr<Collider> other) {
    if (this->parent.hasComponent<Ai>()) {
        auto ai = this->parent.getComponent<Ai>();
        ai->collide(other);
    }
}


void Collider::collide(std::shared_ptr<Collider> other) {
    // Player / Wall collisions: stop player
    if (this->tag == CT_WALL) {
        stop(*other);
    }

    // Enemy / <any> collisions: inform about the collision with <other>
    if (this->tag == CT_ENEMY) {
        notify(other);
    }
}

