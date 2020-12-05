//
// Created by peter on 28.11.20.
//

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

Stick weapon_Stick = Stick();