#include "Stick.h"
#include "../../alg/Rand.h"

Stick::Stick() {}

float Stick::range() {
    return 0.2;
}

int Stick::recharge() {
    return 500;
}

int Stick::power() {
    return damageTable[getRandomRange(0, 9)];
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
