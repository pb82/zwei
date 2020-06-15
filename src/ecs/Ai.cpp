#include "Ai.h"

#include <vector>
#include <iostream>
#include <ASSETS/Assets.h>

#include "../Gfx.h"
#include "../Draw.h"

#include "Entity.h"
#include "Transform.h"
#include "Acceleration.h"
#include "Animation.h"

Ai::Ai(Entity &parent, float delay, std::shared_ptr<Topology> t)
        : Component(parent),
          delay(delay),
          topology(t) {}

void Ai::update(float dt) {
    time += dt;

    if (time < delay) {
        return;
    }

    time = 0.0f;
    plan(dt);
}

void Ai::collide(std::shared_ptr<Collider> other) {
}

void Ai::plan(float dt) {

    auto transform = parent.getComponent<Transform>();
    auto acceleration = parent.getComponent<Acceleration>();
    auto animation = parent.getComponent<Animation>();

    Path p(topology);
    Position goal = {1, 1};
    Position start;
    transform->p.nearestTile(start);

    route.clear();
    if (p.calculate(start, goal, route)) {
        if (route.size() < 1) return;
        auto next = route.at(route.size() - 1);

        float angle = start.angle(next);

        acceleration->trajectory.angle = angle;
        acceleration->maxSpeed = acceleration->resetSpeed;
        animation->paused = false;
    } else {
        acceleration->maxSpeed = 0;
        acceleration->speed = 0;
        animation->paused = true;
    }
}

void Ai::render() {
    auto texture = Assets::instance().getTexture(TILES);

    if (route.size() > 0) {
        for (auto &p: route) {
            SDL_Rect box;

            RT_Camera.project(box, p.x, p.y);

            SDL_Rect source;
            Gfx::pick(source, 53, texture->w);
            Draw::instance().draw(texture->mem, source, box);
        }
    }
}