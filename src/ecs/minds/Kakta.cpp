#include "../Transform.h"

#include "Kakta.h"

Kakta::Kakta(Entity &parent) : Mind(parent) {}

Kakta::~Kakta() {}

int Kakta::delay() {
    return 100;
}

bool Kakta::activate() {
    float distance = getDistanceFromPlayer();
    if (distance < activationRange) {
        return true;
    }

    stop();
    return false;
}

void Kakta::plan(float dt) {
    auto player = RT_Context.getPlayer();

    if (isInRangeForAttack(player)) {
        turn(getAngleTo(player));
        attack();
        return;
    }

    if (alliesInRange(activationRange, trackedAllies)) {
        for (auto enemy: trackedAllies) {
            if (isInRangeForAttack(enemy)) {
                turn(getAngleTo(enemy));
                attack();
                return;
            }

            if (hasDirectPathTo(enemy)) {
                walkTowards(enemy);
                return;
            }
        }
    }

    if (hasDirectPathToPlayer()) {
        walkTowardsPlayer();
        return;
    }

    if (hasRouteToPlayer(route)) {
        followRoute(route);
        return;
    }

    stop();
}
