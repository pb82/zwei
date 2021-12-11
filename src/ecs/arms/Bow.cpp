#include "Bow.h"
#include "../../alg/Rand.h"

Bow::Bow() {}

float Bow::range() {
    return 2;
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
    return 6;
}

bool Bow::isProjectile() {
    return true;
}

float Bow::throwback() {
    return 10.0f;
}

int Bow::getProjectileTile() {
    return 105;
}

void Bow::getParams(float *power, float *weight, float *decay) {
    *power = 12;
    *weight = 80;
    *decay = 15;
}