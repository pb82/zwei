#include "../Transform.h"
#include "../Acceleration.h"
#include "../Animation.h"

#include "Kakta.h"
#include "../Attack.h"

Kakta::Kakta(Entity &parent) : Mind(parent) {}

Kakta::~Kakta() noexcept {}

int Kakta::delay() {
    return 100;
}

bool Kakta::activate() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto t = this->parent.getComponent<Transform>();

    float distance = playerTransform->p.distance(t->p);

    if (distance < activationRange) {
        return true;
    }

    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    animation->stop();
    acceleration->decelerate();

    return false;
}

void Kakta::plan(float dt) {
    auto player = RT_Context.getPlayer();

    auto transform = parent.getComponent<Transform>();
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    auto playerCollider = player->getComponent<Collider>();

    // Try to attack the player if in range
    if (this->parent.hasComponent<Attack>()) {
        if (this->parent.hasComponent<Stats>()) {
            auto attack = this->parent.getComponent<Attack>();
            auto stats = this->parent.getComponent<Stats>();
            if (stats->inventory.hasWeapon()) {
                float distance = playerCollider->tracked->p.distance(transform->p);
                Direction d = acceleration->getDirection();
                float offset = 0.0f;
                // Compensate for hitbox padding
                if (d == N) offset = 1 - playerCollider->padding.top;
                if (d == S) offset = 1 - playerCollider->padding.bottom;
                if (d == E) offset = 1 - playerCollider->padding.left;
                if (d == W) offset = 1 - playerCollider->padding.right;
                if (distance <= (stats->inventory.weapon->range() + offset)) {
                    if (stats->inventory.weapon->isProjectile()) {
                        float angle = transform->p.angle(playerCollider->tracked->p);
                        acceleration->turn(angle);
                        acceleration->decelerate();
                        attack->attack();
                        return;
                    } else {
                        attack->attack();
                        return;
                    }
                }
            }
        }
    }

    route.clear();
    transform->p.bresenham(playerCollider->tracked->p, route);

    if (RT_Context.getTopology().allAccessible(route, transform->p)) {
        float angle = transform->p.angle(playerCollider->tracked->p);
        acceleration->turn(angle);
        acceleration->accelerate();
        animation->start();
        return;
    }

    route.clear();
    Path p(RT_Context.getTopology());

    Position start;
    transform->p.nearestTile(start);
    playerCollider->tracked->p.nearestTile(goal);

    goal = RT_Context.getTopology().nearestAccessible(goal);

    if (p.calculate(start, goal, route)) {
        if (route.size() < 1) return;

        auto next = route.at(route.size() - 1);

        float angle = start.angle(next);

        if (acceleration->sameAngle(angle)) {
            acceleration->decelerate();
        }

        acceleration->turn(angle);
        acceleration->accelerate();
        animation->start();
    } else {
        acceleration->decelerate();
        animation->stop();
    }
}
