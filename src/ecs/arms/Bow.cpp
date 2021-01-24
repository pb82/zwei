#include "Bow.h"
#include "../../alg/Rand.h"

Bow::Bow() {}

float Bow::range() {
    return 5;
}

int Bow::recharge() {
    return 1000;
}

int Bow::damage(const Character &c) {
    int s = (sizeof(damageTable) / sizeof(damageTable[0])) - 1;
    int d = damageTable[Rnd::getRandomRange(0, s)];
    return d * (1 + (c.getDexterity() / 10));
}

float Bow::speed() {
    return 5;
}

bool Bow::isProjectile() {
    return true;
}

float Bow::throwback() {
    return 10.0f;
}

void Bow::projectileFrames(std::vector<int>& target) {
    target.push_back(102);
    target.push_back(103);
}