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

void ColliderGroup::stopInvolved(ColliderTag tag) {
    if (!has(tag)) return;
    for (auto &col : this->involved.at(tag)) {
        col->stop();
    }
}

bool ColliderGroup::has(ColliderTag tag) {
    return this->involved.find(tag) != this->involved.end();
}

void Col::collide(float dt) {
    std::vector<std::shared_ptr<Collider>> colliders;
    Manager::instance().getColliders(colliders);

    std::stack<ColliderGroup> collisions;

    for (auto &a : colliders) {

        if (a->tag == CT_WALL) continue;

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
                b->checked = true;
            }
        }

        if (collisions.top().size == 0) {
            collisions.pop();
        }
    }

    if (!collisions.empty()) {
        while (!collisions.empty()) {
            ColliderGroup &group = collisions.top();

            // An enemy collides with something
            if (group.subject->tag == CT_ENEMY) {
                // Enemy collides with wall: stop it
                if (group.has(CT_WALL)) {
                    group.subject->stop();
                    group.subject->pause(300);
                    for (auto &involved : group.involved.at(CT_WALL)) {
                        involved->kick(group.subject);
                    }
                    if (group.has(CT_ENEMY)) {
                        for (auto &involved : group.involved.at(CT_ENEMY)) {
                            involved->stop();
                            involved->pause(300);
                            involved->kick(group.subject);
                        }
                    }
                }

                // Enemy collides with player: stop it
                if (group.has(CT_PLAYER)) {
                    group.subject->stop();
                    group.subject->pause(100);
                    if (group.has(CT_ENEMY)) {
                        for (auto &involved : group.involved.at(CT_ENEMY)) {
                            involved->stop();
                            involved->pause(1000);
                        }
                    }
                }

                // Enemy collides with other enemy(ies): conflict resulotion strategy
                if (group.has(CT_ENEMY)) {
                    group.subject->stop();
                    for (auto &involved : group.involved.at(CT_ENEMY)) {
                        involved->stop();
                        involved->pause(300);
                        involved->kick(group.subject);
                    }

                }
            }

            if (group.subject->tag == CT_PLAYER) {

                if (group.has(CT_WALL)) {
                    group.subject->stop();
                }

                if (group.has(CT_ENEMY)) {
                    group.subject->stop();
                    for (auto &involved : group.involved.at(CT_ENEMY)) {
                        involved->stop();
                        involved->pause(1000);
                    }
                }
            }

            collisions.pop();
        }
    }
}
