#include "Spider.h"
#include "../Transform.h"
#include "../Acceleration.h"
#include "../Animation.h"

Spider::Spider(Entity &parent) : Mind(parent) {}

Spider::~Spider() {}

int Spider::delay() {
    return 1000;
}

bool Spider::activate() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto skeletonTransform = this->parent.getComponent<Transform>();

    if (playerTransform->p.distance(skeletonTransform->p) < activationRange) {
        return true;
    }

    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    animation->stop();
    acceleration->decelerate();

    return false;
}

void Spider::plan(float dt) {
    auto player = RT_Context.getPlayer();

    if (isInRangeForAttack(player)) {
        turn(getAngleTo(player));
        attack();
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
