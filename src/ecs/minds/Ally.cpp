#include "Ally.h"
#include "../Acceleration.h"
#include "../Animation.h"
#include "../Attack.h"

Ally::Ally(Entity &parent) : Mind(parent) {}

Ally::~Ally() {}

int Ally::delay() {
    return 500;
}

bool Ally::activate() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto t = this->parent.getComponent<Transform>();

    bool joined = RT_Context.getMemory().getBool("ally.joined", false);
    if (!joined) {
        return false;
    }

    float distance = getDistanceFromPlayer();
    if (distance < activationRange) {
        return true;
    }

    stop();
    return false;
}

void Ally::plan(float dt) {
    if (getDistanceFromPlayer() < 1) {
        stop();
    }

    if (hasDirectPathToPlayer()) {
        walkTowardsPlayer();
    } else {
        if (hasRouteToPlayer(route)) {
            followRoute(route);
        } else {
            stop();
        }
    }
}
