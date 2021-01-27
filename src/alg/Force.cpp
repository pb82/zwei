#include "Force.h"

#include <cmath>

Force::Force(float angle, float power) : angle(angle), power(power) {}

Force::Force(float angle,
             float power,
             float decay,
             float weight) : angle(angle), power(power), decay(decay), weight(weight) {}

bool Force::apply(float *x, float *y) {
    //*x += (power / weight) * cos(angle);
    //*y -= (power / weight) * sin(angle);
    float dx = (power / weight) * cos(angle);
    float dy = (power / weight) * sin(angle);
    total += dx;
    total += dy;
    *x += dx;
    *y -= dy;

    if (power <= 0) {
        power = 0;
    }

    return power > 0;
}

void Force::update(float dt) {
    power -= ((dt * decay) / 1000);
}

void Force::set(float angle, float power) {
    this->angle = angle;
    this->power = power;
}