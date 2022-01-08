#include "Npc.h"

#include "Entity.h"
#include "Acceleration.h"
#include "Animation.h"

Npc::Npc(Entity &parent) : Component(parent) {}

void Npc::addTurn(float angle, float duration, float speed) {
    this->turns.emplace(angle, duration, speed);
}

bool Npc::done() {
    return this->_done;
}

void Npc::update(float dt) {
    if (this->turns.empty()) {
        _done = true;
        return;
    };

    this->turns.front().duration -= dt;

    if (this->turns.front().duration <= 0) {
        this->turns.pop();
        return;
    }

    if (!this->parent.hasComponent<Acceleration>()) {
        return;
    }

    auto turn = this->turns.front();
    auto acc = this->parent.getComponent<Acceleration>();
    auto animation = this->parent.getComponent<Animation>();

    acc->turn(turn.angle);
    acc->setSpeed(turn.speed);
    acc->accelerate();
    if (turn.speed > 0) {
        animation->start();
    } else {
        animation->stop();
    }
}