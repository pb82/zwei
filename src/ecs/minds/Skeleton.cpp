#include <ASSETS/Assets.h>
#include "../Transform.h"
#include "../Acceleration.h"
#include "../Animation.h"

#include "Skeleton.h"
#include "../src/Gfx.h"
#include "../src/Draw.h"

Skeleton::Skeleton(Entity &parent) : Mind(parent) {}

Skeleton::~Skeleton() noexcept {}

int Skeleton::delay() {
    return 500;
}

bool Skeleton::activate() {
    auto player = RT_Context.getPlayer();
    auto playerTransform = player->getComponent<Transform>();
    auto skeletonTransform = this->parent.getComponent<Transform>();

    if (playerTransform->p.distance(skeletonTransform->p) < activationRange) {
        return true;
    }

    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    acceleration->maxSpeed = 0;
    acceleration->speed = 0;
    animation->paused = true;

    return false;
}

void Skeleton::collide(std::shared_ptr<Collider> other) {

}

void Skeleton::plan(float dt) {
    auto player = RT_Context.getPlayer();

    auto transform = parent.getComponent<Transform>();
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    auto playerTransform = player->getComponent<Transform>();

    route.clear();
    transform->p.bresenham(playerTransform->p, route);

    if (RT_Context.getTopology().allAccessible(route)) {
        float angle = transform->p.angle(playerTransform->p);
        acceleration->trajectory.angle = angle;
        acceleration->maxSpeed = acceleration->resetSpeed;
        animation->paused = false;
        return;
    }

    route.clear();
    Path p(RT_Context.getTopology());

    Position goal;
    Position start;
    transform->p.nearestTile(start);
    playerTransform->p.nearestTile(goal);

    if (p.calculate(start, goal, route)) {
        if (route.size() < 1) return;

        auto next = route.at(route.size() - 1);

        float angle = start.angle(next);

        if (acceleration->trajectory.angle != angle) {
            acceleration->maxSpeed = 0;
            acceleration->speed = 0;
        }

        acceleration->trajectory.angle = angle;
        acceleration->maxSpeed = acceleration->resetSpeed;
        animation->paused = false;
    } else {
        acceleration->maxSpeed = 0;
        acceleration->speed = 0;
        animation->paused = true;
    }
}

void Skeleton::render() {
    if (route.empty()) return;
    auto texture = Assets::instance().getTexture(TILES);
    for (auto &p: route) {
        SDL_Rect box;
        RT_Camera.project(box, p.x, p.y);
        SDL_Rect source;
        Gfx::pick(source, 53, texture->w);
        Draw::instance().draw(texture->mem, source, box);
    }
}