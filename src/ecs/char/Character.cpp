#include "Character.h"

Character::Character() {}

void Character::serialize(JSON::Value &to) {
    to["character"]["hitpoints"] = this->hitpoints;
    to["character"]["maxHitpoints"] = this->maxHitpoints;
    to["character"]["baseStrength"] = this->baseStrength;
    to["character"]["baseDexterity"] = this->baseDexterity;
    to["character"]["baseDefense"] = this->baseDefense;
}

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
    if (this->hitpoints < 0) {
        this->hitpoints = 0;
    }
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