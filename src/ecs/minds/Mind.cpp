#include "Mind.h"
#include "../Animation.h"
#include "../Acceleration.h"

float Mind::getDistanceFromPlayer() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();
    return transform->p.distance(playerTransform->p);
}

float Mind::getAngleToPlayer() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();
    return transform->p.angle(playerTransform->p);
}

bool Mind::hasDirectPathToPlayer() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();

    std::vector<Position> route;
    transform->p.bresenham(playerTransform->p, route);
    return RT_Context.getTopology().allAccessible(route, transform->p);
}

void Mind::stop() {
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();
    animation->stop();
    acceleration->decelerate();
}

void Mind::walkTowardsPlayer() {
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();
    float angle = getAngleToPlayer();
    acceleration->turn(angle);
    acceleration->accelerate();
    animation->start();
}

bool Mind::hasRouteToPlayer(std::vector<Position> &route) {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();

    route.clear();
    Path p(RT_Context.getTopology());

    Position start;
    Position goal;

    transform->p.nearestTile(start);
    playerTransform->p.nearestTile(goal);

    goal = RT_Context.getTopology().nearestAccessible(goal);
    return p.calculate(start, goal, route) && route.size() > 1;
}

void Mind::followRoute(std::vector<Position> &route) {
    auto next = route.at(route.size() - 1);
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();

    Position start;
    transform->p.nearestTile(start);

    float angle = start.angle(next);

    if (acceleration->sameAngle(angle)) {
        acceleration->decelerate();
    }

    acceleration->turn(angle);
    acceleration->accelerate();
    animation->start();
}