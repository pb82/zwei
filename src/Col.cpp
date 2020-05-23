#include "Col.h"

#include <SDL2/SDL.h>

#include "ecs/Manager.h"

void Col::collide() {
    std::vector<std::shared_ptr<Collider>> colliders;
    Manager::instance().getColliders(colliders);
    SDL_Rect intersection;

    for (auto &a : colliders) {
        if (a->checked) continue;
        for (auto &b : colliders) {
            if (a == b) continue;
            if (a->checked) continue;
            if (SDL_IntersectRect(&a->boundingBox, &b->boundingBox, &intersection)) {
                a->collide(b, intersection);
                a->checked = true;
                b->checked = true;
            }
        }
    }
}