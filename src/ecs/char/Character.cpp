#include "Character.h"

Character::Character() {}

void Character::serialize(JSON::Value &to) {
    to["player"]["hitpoints"] = this->hitpoints;
    to["player"]["maxHitpoints"] = this->maxHitpoints;
    to["player"]["baseStrength"] = this->baseStrength;
    to["player"]["baseDexterity"] = this->baseDexterity;
    to["player"]["baseDefense"] = this->baseDefense;
}

void Character::deserialize(JSON::Value &from) {
    this->hitpoints = from["player"]["hitpoints"].as<int>();
    this->maxHitpoints = from["player"]["maxHitpoints"].as<int>();
    this->baseStrength = from["player"]["baseStrength"].as<int>();
    this->baseDexterity = from["player"]["baseDexterity"].as<int>();
    this->baseDefense = from["player"]["baseDefense"].as<int>();
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

void Character::bumpHitpoints() {
    this->maxHitpoints = this->maxHitpoints += BAR_HITPOINTS;
    this->hitpoints = this->maxHitpoints;
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

std::tuple<int, int> Character::getHitpointsPerHeart() {
    int hp = this->hitpoints % BAR_HITPOINTS;
    if (hp == 0 && this->hitpoints > 0) {
        hp = BAR_HITPOINTS;
    }
    return {hp, BAR_HITPOINTS};
}

std::tuple<int, int> Character::getHearts() {
    int total = this->maxHitpoints / BAR_HITPOINTS;
    int full = this->hitpoints / BAR_HITPOINTS;
    return {full, total - full};
}

bool Character::dead() const {
    return this->hitpoints <= 0;
}
