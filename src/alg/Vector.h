#ifndef ZWEI_VECTOR_H
#define ZWEI_VECTOR_H

#include <vector>
#include <cmath>

#include "Direction.h"

#define VM_0_PI              0
#define VM_25_PI             (M_PI / 4)
#define VM_50_PI             (M_PI / 2)
#define VM_75_PI             ((3 * M_PI) / 4)
#define VM_100_PI            M_PI
#define VM_125_PI            ((5 * M_PI) / 4)
#define VM_150_PI            ((3 * M_PI) / 2)
#define VM_175_PI            ((7 * M_PI) / 4)
#define VM_200_PI            M_PI * 2
#define VM_BETWEEN(x, s, l)  (x > s && x < l)

class Vector {
public:
    Vector(float radius, float angle);

    Vector(const Vector &other) noexcept {
        this->radius = other.radius;
        this->angle = other.angle;
    }

    Vector(const Vector &&other) noexcept
            : radius(other.radius),
              angle(other.angle) {}

    Vector &operator=(const Vector &other) {
        this->radius = other.radius;
        this->angle = other.angle;
        return *this;
    }

    void translate(float *x, float *y);

    Direction getDirection();

    float normalize();

    float radius;
    float angle;

};

#endif
