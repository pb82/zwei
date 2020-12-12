#include "Stick.h"

Stick::Stick() {}

float Stick::range() {
    return 0.5;
}

int Stick::recharge() {
    return 500;
}

float Stick::power() {
    return 50;
}

float Stick::speed() {
    return 2;
}

bool Stick::isProjectile() {
    return false;
}

float Stick::throwback() {
    return 5.0f;
}

Stick weapon_Stick = Stick();