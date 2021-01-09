#include "Trigger.h"

#include <SDL2/SDL.h>

#include "../Rt.h"
#include "Collider.h"

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
    bool collides = SDL_HasIntersection(&a->boundingBox, &b->boundingBox);

    if (!active && collides) {
        active = true;
        if (enter) enter();
    } else if (active && !collides) {
        active = false;
        if (exit) exit();
    }
}