#include "Bow.h"
#include "../../alg/Rand.h"

Bow::Bow() {}

float Bow::range() {
    return 4;
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
    return 16;
}

bool Bow::isRanged() {
    return true;
}

float Bow::throwback() {
    return 20.0f;
}

int Bow::getProjectileTile() {
    return 105;
}

void Bow::getParams(float *power, float *weight, float *decay) {
    *power = 15;
    *weight = 60;
    *decay = 10;
}