#include "Character.h"

Character::Character() {}

void Character::setBase(int hp, int str, int dex, int def) {
    this->maxHitpoints = hp;
    this->baseStrength = str;
    this->baseDexterity = dex;
    this->baseDefense = def;
    this->hitpoints = hp;
}

int Character::damage(int d, bool isProjectile) {
    d -= baseDefense / 2;
    if (isProjectile) {
        d -= baseDexterity / 5;
    }

    bool miss = (rand() % 100) < (baseDexterity / 2);
    if (miss) {
        d = 0;
    }

    if (d < 0) d = 0;
    this->hitpoints -= d;
    return d;
}

int Character::getStrength() const {
    return baseStrength;
}

int Character::getDexterity() const {
    return baseDexterity;
}

std::tuple<int, int> Character::getHitpoints() {
    return {this->hitpoints, this->maxHitpoints};
}

bool Character::dead() const {
    return this->hitpoints <= 0;
}