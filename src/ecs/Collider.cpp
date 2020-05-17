#include "Collider.h"

#include "Entity.h"
#include "../Draw.h"

Collider::Collider(Entity &parent, std::shared_ptr<Transform> tracked, float dx, float dy) :
        Component(parent),
        tracked(tracked),
        dx(dx),
        dy(dy) {
    updateBoundingBox();
}

void Collider::updateBoundingBox() {
    RT_Camera.project(boundingBox, tracked->p.x, tracked->p.y);
}

void Collider::update(float dt) {
    updateBoundingBox();
}
