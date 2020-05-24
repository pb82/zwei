#include "Col.h"

#include <SDL2/SDL.h>

#include "ecs/Manager.h"

void Col::collide(float dt) {
    std::vector<std::shared_ptr<Collider>> colliders;
    Manager::instance().getColliders(colliders);

    for (auto &a : colliders) {
        for (auto &b : colliders) {
            if (a == b) continue;
            a->update(dt);
            b->update(dt);
            if (SDL_HasIntersection(&a->boundingBox, &b->boundingBox)) {
                a->collide(b);
            }
        }
    }
}