#include "Stone.h"
#include "../../alg/Rand.h"

Stone::Stone() {}

float Stone::range() {
    return 1.7;
}

int Stone::recharge() {
    return 3000;
}

int Stone::damage(const Character &c) {
    int s = (sizeof(damageTable) / sizeof(damageTable[0])) - 1;
    int d = damageTable[Rnd::getRandomRange(0, s)];
    return d * (1 + (c.getDexterity() / 10));
}

float Stone::speed() {
    return 5;
}

bool Stone::isProjectile() {
    return true;
}

float Stone::throwback() {
    return 10.0f;
}

int Stone::getProjectileTile() {
    return 106;
}

void Stone::getParams(float *power, float *weight, float *decay) {
    *power = 10;
    *weight = 70;
    *decay = 25;
}