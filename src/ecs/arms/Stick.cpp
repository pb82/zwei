//
// Created by peter on 28.11.20.
//

#include "Stick.h"

Stick::Stick() {}

float Stick::range() {
    return 0.5;
}

int Stick::recharge() {
    return 1000;
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

Stick weapon_Stick = Stick();