#include "Stick.h"

Stick::Stick() {}

float Stick::range() {
    return 0.2;
}

int Stick::recharge() {
    return 500;
}

float Stick::power() {
    return 20;
}

float Stick::speed() {
    return 5;
}

bool Stick::isProjectile() {
    return false;
}

float Stick::throwback() {
    return 15.0f;
}

Stick weapon_Stick = Stick();