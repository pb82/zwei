#include <SDL.h>

#include "Interactible.h"
#include "Transform.h"
#include "Collider.h"
#include "Acceleration.h"
#include "../Rt.h"

Interactible::Interactible(Entity &parent) : Component(parent) {}

void Interactible::onInteract(interact_Fn cb) {
    this->fn = cb;
}

void Interactible::update(float dt) {}

void Interactible::interact() {
    auto t = this->parent.getComponent<Transform>();
    auto p = RT_Player->getComponent<Transform>();
    auto a = RT_Player->getComponent<Acceleration>();
    auto playerCollider = RT_Player->getComponent<Collider>();
    auto itemCollider = this->parent.getComponent<Collider>();

    float distance = t->p.distance(p->p);

    // enforce min and max distance for interaction
    if (distance >= 0.3 && distance <= 1.1) {
        // The player is standing inside the interactible object
        // This could trap the player when e.g. a door is closed
        // Correct the position to never overlap with the other
        // object
        if (SDL_HasIntersection(&playerCollider->boundingBox, &itemCollider->boundingBox)) {
            switch (a->getDirection()) {
                case N:
                    p->p.y = std::ceil(p->p.y);
                    break;
                case S:
                    p->p.y = std::floor(p->p.y);
                    break;
                case E:
                    p->p.x = std::floor(p->p.x);
                    break;
                case W:
                    p->p.x = std::ceil(p->p.x);
                    break;
                case NONE:
                default:
                    break;
            }
        }

        this->fn(this->parent, internalState);
    }
}