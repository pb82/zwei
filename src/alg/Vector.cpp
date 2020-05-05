#include "Vector.h"

#include <cmath>

Vector::Vector(float radius, float angle) : radius(radius), angle(angle) {}

void Vector::translate(float *x, float *y) {
    float h = *x;
    float k = *y;
    *x = radius * cos(angle) + h;
    *y = radius * sin(angle) + k;
}

Direction Vector::getDirection() {
    if (BETWEEN(angle, M_25_PI, M_75_PI)) return N;
    if (BETWEEN(angle, M_75_PI, M_125_PI)) return W;
    if (BETWEEN(angle, M_125_PI, M_175_PI)) return S;
    return E;
}