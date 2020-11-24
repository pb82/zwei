#include "Col.h"

#include <SDL2/SDL.h>

#include <stack>

#include "ecs/Manager.h"
#include "ecs/Acceleration.h"

void ColliderGroup::collidesWith(std::shared_ptr<Collider> colliding) {
    size++;
    if (involved.find(colliding->tag) == involved.end()) {
        involved.emplace(colliding->tag, std::vector<std::shared_ptr<Collider>>{});
    }
    involved[colliding->tag].push_back(colliding);
}

void Col::collide(float dt) {
    std::vector<std::shared_ptr<Collider>> colliders;
    Manager::instance().getColliders(colliders);

    std::stack<ColliderGroup> collisions;

    for (auto &a : colliders) {

        ColliderGroup group(a);
        collisions.push(group);

        for (auto &b : colliders) {
            if (a == b) continue;
            if (a->checked) continue;
            if (a->tag == CT_WALL && b->tag == CT_WALL) continue;
            a->update(dt);
            b->update(dt);
            if (SDL_HasIntersection(&a->boundingBox, &b->boundingBox)) {
                collisions.top().collidesWith(b);
                // b->checked = true;
            }
        }

        if (collisions.top().size == 0) {
            collisions.pop();
        }
    }

    if (!collisions.empty()) {
        while (!collisions.empty()) {
            ColliderGroup &group = collisions.top();

            // Whatever collides with a wall gets stopped
            if (group.subject->tag == CT_WALL) {
                if (group.involved.find(CT_PLAYER) != group.involved.end()) {
                    for (auto &involved : group.involved.at(CT_PLAYER)) {
                        involved->stop();
                    }
                }
            }

            if (group.subject->tag == CT_ENEMY) {
                if (group.involved.find(CT_ENEMY) != group.involved.end()) {
                    // group.subject->stop();
                    for (auto &involved : group.involved.at(CT_ENEMY)) {
                        involved->stop();
                    }
                }
            }

            if (group.subject->tag == CT_PLAYER) {
                if (group.involved.find(CT_ENEMY) != group.involved.end()) {
                    for (auto &involved : group.involved.at(CT_ENEMY)) {
                        involved->stop();
                    }
                }
            }

            collisions.pop();
        }
    }

    /*
    for (auto &a : colliders) {
        if (!RT_Camera.visible(a->boundingBox)) continue;
        for (auto &b : colliders) {
            if (a == b) continue;
            if (!RT_Camera.visible(b->boundingBox)) continue;
            a->update(dt);
            b->update(dt);
            if (SDL_HasIntersection(&a->boundingBox, &b->boundingBox)) {
                a->collide(b);
            }
        }
    }
    */
}
