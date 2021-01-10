#include "Trigger.h"

#include <SDL2/SDL.h>

#include "../Rt.h"
#include "Collider.h"
#include "Acceleration.h"

Trigger::Trigger(Entity &parent) : Component(parent) {}

void Trigger::onEnter(trigger_Fn cb) {
    this->enter = cb;
}

void Trigger::onExit(trigger_Fn cb) {
    this->exit = cb;
}

void Trigger::update(float dt) {
    auto player = RT_Context.getPlayer();
    auto a = player->getComponent<Collider>();
    auto b = parent.getComponent<Collider>();
    auto c = player->getComponent<Acceleration>();
    bool collides = SDL_HasIntersection(&a->boundingBox, &b->boundingBox);

    if (!active && collides) {
        active = true;
        if (enter) enter(c->getAngle());
    } else if (active && !collides) {
        active = false;
        if (exit) exit(c->getAngle());
    }
}