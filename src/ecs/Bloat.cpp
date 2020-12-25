#include "Bloat.h"
#include "Entity.h"
#include "Transform.h"
#include "Collectable.h"
#include "SelfDestruct.h"

Bloat::Bloat(Entity &parent) : Component(parent) {}

void Bloat::update(float dt) {
    auto transform = this->parent.getComponent<Transform>();
    transform->padding.top -= (1 / dt);
    transform->padding.bottom -= (1 / dt);
    transform->padding.left -= (1 / dt);
    transform->padding.right -= (1 / dt);

    auto collect = this->parent.getComponent<Collectable>();
    auto sd = this->parent.getComponent<SelfDestruct>();
    float dx = dt * (255 / sd->value);

    if (collect->alpha <= dx) {
        collect->alpha = 0;
        return;
    }

    collect->alpha-= dx;
}