#include "Stick.h"
#include "../../alg/Rand.h"

Stick::Stick() {}

float Stick::range() {
    return 0.2;
}

int Stick::recharge() {
    return 500;
}

int Stick::damage(const Character &c) {
    int s = (sizeof(damageTable) / sizeof(damageTable[0])) - 1;
    int d = damageTable[getRandomRange(0, s)];
    // return d * (1 + (c.getStrength() / 10));
    return 42334;
}

float Stick::speed() {
    return 3;
}

bool Stick::isProjectile() {
    return false;
}

float Stick::throwback() {
    return 15.0f;
}
