#include "Vector.h"

#include <cmath>

Vector::Vector(float radius, float angle) : radius(radius), angle(angle) {}

void Vector::translate(float *x, float *y) {
    *x += radius * cos(angle);
    *y -= radius * sin(angle);
}

Direction Vector::getDirection() {
    int compass = (((int) round(angle / (2 * VM_100_PI / 8))) + 8) % 8;
    switch (compass) {
        case 0:
        case 1:
        case 7:
            return E;
        case 2:
            return N;
        case 3:
        case 4:
        case 5:
            return W;
        case 6:
            return S;
        default:
            return S;
    }
}
