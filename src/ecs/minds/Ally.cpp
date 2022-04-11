#include "Ally.h"

Ally::Ally(Entity &parent) : Mind(parent) {}

Ally::~Ally() {}

int Ally::delay() {
    return 500;
}

bool Ally::activate() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto t = this->parent.getComponent<Transform>();

    float distance = getDistanceFromPlayer();
    if (distance < activationRange) {
        return true;
    }

    stop();
    return false;
}

void Ally::plan(float dt) {
    if (enemiesInRange(5, trackedEnemies)) {
        for (auto enemy: trackedEnemies) {
            if (isInRangeForAttack(enemy)) {
                turn(getAngleTo(enemy));
                attack();
                return;
            }

            if (hasDirectPathTo(enemy)) {
                walkTowards(enemy);
                return;
            }

            if (hasRouteTo(route, enemy)) {
                followRoute(route);
            }
        }
    }

    if (getDistanceFromPlayer() <= 1.3) {
        stop();
        return;
    }

    if (hasDirectPathToPlayer()) {
        walkTowardsPlayer();
        return;
    }

    if (hasRouteToPlayer(route)) {
        followRoute(route);
    } else {
        stop();
    }
}
