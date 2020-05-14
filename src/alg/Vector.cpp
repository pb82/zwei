#include "Vector.h"

#include <cmath>

Vector::Vector(float radius, float angle) : radius(radius), angle(angle) {}

void Vector::translate(float *x, float *y) {
    *x += radius * cos(angle);
    *y -= radius * sin(angle);
}

Direction Vector::getDirection() {
    if (BETWEEN(angle, VM_25_PI, VM_75_PI)) return N;
    if (BETWEEN(angle, VM_75_PI, VM_125_PI)) return W;
    if (BETWEEN(angle, VM_125_PI, VM_175_PI)) return S;
    return E;
}