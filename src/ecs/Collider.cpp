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

void Collider::stop(const Collider &c) {
    if (c.parent.hasComponent<Acceleration>()) {
        auto acceleration = c.parent.getComponent<Acceleration>();
        c.tracked->p = acceleration->last;
    }
}

void Collider::notify(const Collider &c) {
    if (c.parent.hasComponent<Ai>()) {
        auto ai = c.parent.getComponent<Ai>();
        ai->collide(std::make_shared<Collider>(*this));
    }
}

void Collider::collideWall(std::shared_ptr<Collider> other, float bounce) {
    stop(*other);
    notify(*other);
}

void Collider::collideEnemy(std::shared_ptr<Collider> other) {
    stop(*other);

    if (other->tag == CT_PROJECTILE) {
        auto a = this->parent.getComponent<Acceleration>();
        a->applyForce(VM_0_PI, 20);
        other->parent.disable();
    } else {
        stop(*this);
    }

    notify(*other);
    notify(*this);
}

void Collider::collide(std::shared_ptr<Collider> other) {
    if (this->tag == CT_WALL) {
        collideWall(other, 0.0f);
    } else if (this->tag == CT_ENEMY) {
        collideEnemy(other);
    }
}

