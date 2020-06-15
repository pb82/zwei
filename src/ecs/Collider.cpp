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

void Collider::collideWall(std::shared_ptr<Collider> other, float bounce) {
    if (other->parent.hasComponent<Acceleration>()) {
        auto acceleration = other->parent.getComponent<Acceleration>();
        other->tracked->p = acceleration->last;

        if (bounce > 0) {
            // Bounce in opposite direction
            float angle = acceleration->trajectory.angle - M_PI;
            acceleration->applyForce(angle, bounce);
        }
    }

    if (other->parent.hasComponent<Ai>()) {
        auto ai = other->parent.getComponent<Ai>();
        ai->collide(std::make_shared<Collider>(*this));
    }
}

void Collider::collide(std::shared_ptr<Collider> other) {
    if (this->tag == CT_WALL) {
        collideWall(other, 0.0f);
    }
}

