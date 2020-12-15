#include "Spider.h"
#include "../Transform.h"
#include "../Acceleration.h"
#include "../Animation.h"

Spider::Spider(Entity &parent) : Mind(parent) {}

Spider::~Spider() noexcept {}

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

    auto transform = parent.getComponent<Transform>();
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    auto playerTransform = player->getComponent<Transform>();

    route.clear();
    transform->p.bresenham(playerTransform->p, route);

    if (RT_Context.getTopology().allAccessible(route, transform->p)) {
        float angle = transform->p.angle(playerTransform->p);
        acceleration->turn(angle);
        acceleration->accelerate();
        animation->start();
        return;
    }

    acceleration->decelerate();
    animation->stop();
}
