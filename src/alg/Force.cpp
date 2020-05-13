#include "Force.h"

#include <cmath>

Force::Force(float angle, float power) : angle(angle), power(power) {}

bool Force::apply(float weight, float *x, float *y) {
    *x = (power / weight) * cos(angle) + *x;
    *y = (power / weight) * sin(angle) + *y;
    return power <= 0;
}

void Force::update(float weight, float dt) {
    power -= ((dt * weight) / 1000);
}
