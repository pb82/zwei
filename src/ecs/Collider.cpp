#include "Collider.h"

#include "Entity.h"
#include "Acceleration.h"

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

Direction Collider::inRelationTo(std::shared_ptr<Collider> other) {
    float mx = (this->boundingBox.x + (this->boundingBox.x + this->boundingBox.w) / 2);
    float my = (this->boundingBox.y + (this->boundingBox.y + this->boundingBox.h) / 2);
    float ox = (other->boundingBox.x + (other->boundingBox.x + other->boundingBox.w) / 2);
    float oy = (other->boundingBox.y + (other->boundingBox.y + other->boundingBox.h) / 2);

}

void Collider::collide(std::shared_ptr<Collider> other, SDL_Rect &intersection) {
    if (this->tag == CT_WALL) {
        if (other->parent.hasComponent<Acceleration>()) {
            auto acceleration = other->parent.getComponent<Acceleration>();
            auto transform = other->parent.getComponent<Transform>();

            if (intersection.w > intersection.h) {
                if (intersection.y <= other->boundingBox.y) {
                    if (acceleration->trajectory.getDirection() == N) {
                        acceleration->speed = 0;
                        transform->p.y += RT_Camera.scale(intersection.h);
                    }
                } else if (intersection.y > other->boundingBox.y) {
                    if (acceleration->trajectory.getDirection() == S) {
                        acceleration->speed = 0;
                    }
                }
            } else if (intersection.w < intersection.h) {
                if (intersection.x <= other->boundingBox.x) {
                    if (acceleration->trajectory.getDirection() == W) {
                        acceleration->speed = 0;
                    }
                } else if (intersection.x > other->boundingBox.x) {
                    if (acceleration->trajectory.getDirection() == E) {
                        acceleration->speed = 0;
                    }
                }
            } else {
                acceleration->applyForce(acceleration->trajectory.angle, 5);
            }
        }
    }
}

