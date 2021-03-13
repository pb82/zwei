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

    // enforce min distance for interaction
    if (t->p.distance(p->p) <= 1) {
        this->fn(this->parent);
    }
}