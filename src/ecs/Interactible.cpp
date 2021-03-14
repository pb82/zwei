#include "Interactible.h"
#include "Transform.h"
#include "../Rt.h"

Interactible::Interactible(Entity &parent) : Component(parent) {}

void Interactible::onInteract(interact_Fn cb) {
    this->fn = cb;
}

void Interactible::update(float dt) {}

void Interactible::interact() {
    auto t = this->parent.getComponent<Transform>();
    auto p = RT_Player->getComponent<Transform>();

    float distance = t->p.distance(p->p);

    // enforce min and max distance for interaction
    if (distance >= 0.5 && distance <= 1.2) {
        this->fn(this->parent);
    }
}