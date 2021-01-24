#include "Character.h"

Character::Character() {}

void Character::adjustToLevel() {
    this->experienceToNextLevel = level * 100;
    this->experienceToNextLevel *= 1 + (level / 10);
    this->maxHitpoints = this->baseHitpoints + (10 * level);
    this->maxHitpoints *= 1 + (level / 100);
    this->hitpoints = this->maxHitpoints;
}

bool Character::collectExperience(int points) {
    this->experience += points;
    bool levelUp = false;

    while (this->experience >= this->experienceToNextLevel) {
        this->level++;
        this->adjustToLevel();
        levelUp = true;
    }
    return levelUp;
}

void Character::setBase(int hp, int str, int dex, int def) {
    this->baseHitpoints = hp;
    this->baseStrength = str;
    this->baseDexterity = dex;
    this->baseDefense = def;
    adjustToLevel();
}

void Character::setLevel(int level) {
    this->level = level;
    adjustToLevel();
}

int Character::releaseExperience() const {
    int factor = (rand() % 20);
    return level * factor;
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

std::tuple<int, int> Character::getExperience() {
    return {this->experience, this->experienceToNextLevel};
}

std::tuple<int, int> Character::getHitpoints() {
    return {this->hitpoints, this->maxHitpoints};
}

bool Character::dead() const {
    return this->hitpoints <= 0;
}