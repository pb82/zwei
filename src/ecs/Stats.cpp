#include "Stats.h"

Stats::Stats(Entity &parent) : Component(parent) {}

void Stats::equipWeapon(std::shared_ptr<Weapon> w) {
    this->weapon = w;
}

void Stats::dropWeapon() {
    this->weapon = nullptr;
}

bool Stats::hasWeapon() {
    if (this->weapon) {
        return true;
    }
    return false;
}