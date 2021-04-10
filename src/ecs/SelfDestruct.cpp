#include "SelfDestruct.h"
#include "Entity.h"
#include "Acceleration.h"

SelfDestruct::SelfDestruct(Entity &parent, SelfDestructType type, float value)
        : type(type), value(value), Component(parent) {}

bool SelfDestruct::serialize(JSON::Value &to) {
    to["type"] = this->type;
    to["current"] = this->current;
    to["value"] = this->value;
    return true;
}

void SelfDestruct::update(float dt) {
    if (type == STILL) {
        auto a = parent.getComponent<Acceleration>();
        if (a->hasForces() == false) {
            parent.disable();
        }
        return;
    }
    if (type == TIMER) {
        current += dt;
    } else {
        updateDistance(dt);
    }
    if (current >= value) {
        parent.disable();
    }
}

void SelfDestruct::updateDistance(float dt) {
    auto a = parent.getComponent<Acceleration>();
    current += (dt / 1000) * a->speed;
}