#include <ASSETS/Assets.h>
#include "../Transform.h"
#include "../Acceleration.h"
#include "../Animation.h"

#include "Skeleton.h"
#include "../src/Gfx.h"
#include "../src/Draw.h"
#include "../../Debug.h"
#include "../../alg/Rand.h"

Skeleton::Skeleton(Entity &parent) : Mind(parent) {}

Skeleton::~Skeleton() noexcept {}

int Skeleton::delay() {
    return 50;
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

    animation->stop();
    acceleration->decelerate();

    return false;
}

void Skeleton::collide(std::shared_ptr<Collider> other) {
    if (other->tag == CT_ENEMY) {
        auto acceleration = parent.getComponent<Acceleration>();
        float currentAngle = acceleration->getAngle();

        // Random direction
        acceleration->turn(currentAngle + getRandomFloat(VM_100_PI));
        return;
    }

    if (other->tag == CT_PLAYER) {
        auto acceleration = parent.getComponent<Acceleration>();
        acceleration->decelerate();
    }
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
        acceleration->turn(angle);
        acceleration->accelerate();
        animation->start();
        return;
    }

    route.clear();
    Path p(RT_Context.getTopology());

    Position start;
    transform->p.nearestTile(start);
    playerTransform->p.nearestTile(goal);

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

void Skeleton::render() {
    if (!Debug::drawBoundingBoxes) return;

    /*
    auto texture = Assets::instance().getTexture(TILES);

    SDL_Rect box;
    RT_Camera.project(box, goal.x, goal.y);
    SDL_Rect source;
    Gfx::pick(source, 54, texture->w);
    Draw::instance().draw(texture->mem, source, box);

    if (route.empty()) return;
    for (auto &p: route) {
        SDL_Rect box;
        RT_Camera.project(box, p.x, p.y);
        SDL_Rect source;
        Gfx::pick(source, 53, texture->w);
        Draw::instance().draw(texture->mem, source, box);
    }
    */
}