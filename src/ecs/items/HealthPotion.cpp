#include "HealthPotion.h"

HealthPotion::HealthPotion() : Item(HEALTH_POTION) {}

bool HealthPotion::stackable() {
    return true;
}

int HealthPotion::tile() {
    return 93;
}
