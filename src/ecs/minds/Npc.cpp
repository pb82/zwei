#include "Npc.h"
#include "../Acceleration.h"
#include "../Animation.h"

Npc::Npc(Entity &parent) : Mind(parent) {}

void Npc::go() {
    this->start = true;
}

void Npc::addTurn(float angle, float duration, float speed) {
    this->turns.push_back(std::make_shared<NpcTurn>(angle, duration, speed));
}

bool Npc::activate() {
    return true;
}

int Npc::delay() {
    return 0;
}

void Npc::onFinish(std::function<void()> cb) {
    this->_finish = cb;
}

void Npc::plan(float dt) {
    if (!start) return;

    auto acc = this->parent.getComponent<Acceleration>();
    auto anim = this->parent.getComponent<Animation>();

    if (turns.empty() || index >= turns.size()) {
        acc->decelerate();
        this->parent.disable();
        if (_finish) {
            _finish();
        }
        return;
    }

    auto turn = turns.at(index);
    if (turn->duration <= 0) {
        index++;
        return;
    }

    turn->duration -= dt;

    anim->start();
    acc->turn(turn->angle);
    acc->setSpeed(turn->speed);
    acc->accelerate();
}