#include "Caterpillar.h"
#include "../Acceleration.h"
#include "../Animation.h"
#include "../../alg/Rand.h"

Caterpillar::Caterpillar(Entity &parent) : Mind(parent) {}

Caterpillar::~Caterpillar() {}

int Caterpillar::delay() {
    return 100;
}

bool Caterpillar::activate() {
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

void Caterpillar::plan(float dt) {
    auto player = RT_Context.getPlayer();
    auto acceleration = this->parent.getComponent<Acceleration>();

    if (hasDirectPathToPlayer()) {
        walkTowardsPlayer();
    }

    if (hasRouteToPlayer(route)) {
        followRoute(route);
        return;
    }

    /*
    if (isInRangeForAttack(player)) {
        float angle = getAngleTo(player);
        turn(angle);
        attack();
        return;
    }

    walkTowardsPlayer();
    */
}
