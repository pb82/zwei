#include "Mind.h"
#include "../Animation.h"
#include "../Acceleration.h"
#include "../Attack.h"

float Mind::getDistanceFromPlayer() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();
    return transform->p.distance(playerTransform->p);
}

float Mind::getDistanceFrom(std::shared_ptr<Entity> entity) {
    auto transform = this->parent.getComponent<Transform>();
    auto other = entity->getComponent<Collider>();
    return transform->p.distance(other->tracked->p);
}

float Mind::getAngleTo(std::shared_ptr<Entity> entity) {
    auto transform = this->parent.getComponent<Transform>();
    auto other = entity->getComponent<Transform>();
    return transform->p.angle(other->p);
}

bool Mind::hasDirectPathToPlayer() {
    auto player = RT_Context.getPlayer();
    return hasDirectPathTo(player);
}

bool Mind::hasDirectPathTo(std::shared_ptr<Entity> entity) {
    auto transform = this->parent.getComponent<Transform>();
    auto otherTransform = entity->getComponent<Transform>();

    std::vector<Position> route;
    transform->p.bresenham(otherTransform->p, route);
    return RT_Context.getTopology().allAccessible(route, transform->p);
}

void Mind::stop() {
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();
    animation->stop();
    acceleration->decelerate();
}

void Mind::walkTowardsPlayer() {
    auto player = RT_Context.getPlayer();
    return walkTowards(player);
}

void Mind::walkTowards(std::shared_ptr<Entity> entity) {
    auto animation = parent.getComponent<Animation>();
    auto acceleration = parent.getComponent<Acceleration>();
    float angle = getAngleTo(entity);
    acceleration->turn(angle);
    acceleration->accelerate();
    animation->start();
}

bool Mind::hasRouteToPlayer(std::vector<Position> &route) {
    auto player = RT_Context.getPlayer();
    return hasRouteTo(route, player);
}

bool Mind::hasRouteTo(std::vector<Position> &route, std::shared_ptr<Entity> entity) {
    if (!entity->hasComponent<Transform>()) return false;
    auto otherTransform = entity->getComponent<Transform>();
    auto transform = this->parent.getComponent<Transform>();

    route.clear();
    Path p(RT_Context.getTopology());

    Position start;
    Position goal;

    transform->p.nearestTile(start);
    otherTransform->p.nearestTile(goal);

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

void Mind::turn(float angle) {
    auto acceleration = parent.getComponent<Acceleration>();
    acceleration->turn(angle);
    acceleration->decelerate();
}

void Mind::attack() {
    auto attack = parent.getComponent<Attack>();
    attack->attack();
}

bool Mind::isInRangeForAttack(std::shared_ptr<Entity> entity, float range) {
    float distance = getDistanceFrom(entity);
    return range <= distance;
}

bool Mind::isInRangeForAttack(std::shared_ptr<Entity> entity) {
    if (!parent.hasComponent<Attack>()) return false;
    if (!parent.hasComponent<Stats>()) return false;
    if (!parent.hasComponent<Acceleration>()) return false;
    if (!parent.hasComponent<Transform>()) return false;
    if (!entity->hasComponent<Collider>()) return false;

    auto attack = parent.getComponent<Attack>();
    auto stats = parent.getComponent<Stats>();
    auto acceleration = parent.getComponent<Acceleration>();
    auto transform = parent.getComponent<Transform>();
    auto otherCollider = entity->getComponent<Collider>();

    if (!stats->inventory.hasWeapon()) return false;

    float distance = getDistanceFrom(entity);
    float offset = 0.0f;
    Direction d = acceleration->getDirection();

    // Compensate for hitbox padding
    if (d == N) offset = 1 - otherCollider->padding.bottom / 2;
    if (d == S) offset = 1 - otherCollider->padding.top / 2;
    if (d == E) offset = 1 - otherCollider->padding.left;
    if (d == W) offset = 1 - otherCollider->padding.right;

    float effectiveRange = stats->inventory.weapon->range() + offset;
    bool result = distance <= effectiveRange;
    return result;
}

bool Mind::enemiesInRange(float distance, std::vector<std::shared_ptr<Entity>> &enemies) {
    enemies.clear();
    std::vector<std::shared_ptr<Entity>> allEnemies;
    Manager::instance().getEnemies(allEnemies);
    for (auto &enemy: allEnemies) {
        if (getDistanceFrom(enemy) <= distance) {
            enemies.push_back(enemy);
        }
    }
    return enemies.size() > 0;
}

bool Mind::alliesInRange(float distance, std::vector<std::shared_ptr<Entity>> &allies) {
    allies.clear();
    std::vector<std::shared_ptr<Entity>> allAllies;
    Manager::instance().getAllies(allAllies);
    allAllies.push_back(RT_Context.getPlayer());
    for (auto &enemy: allAllies) {
        if (getDistanceFrom(enemy) <= distance) {
            allies.push_back(enemy);
        }
    }
    return allies.size() > 0;
}