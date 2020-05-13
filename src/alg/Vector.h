#ifndef ZWEI_VECTOR_H
#define ZWEI_VECTOR_H

#include <vector>

#include "Direction.h"

#define M_0_PI              0
#define M_25_PI             (M_PI / 4)
#define M_50_PI             (M_PI / 2)
#define M_75_PI             ((3 * M_PI) / 4)
#define M_125_PI            ((5 * M_PI) / 4)
#define M_150_PI            ((3 * M_PI) / 2)
#define M_175_PI            ((7 * M_PI) / 4)
#define BETWEEN(x, s, l)    (x >= s && x <= l)

class Vector {
public:
    Vector(float radius, float angle);

    void translate(float *x, float *y);

    Direction getDirection();

    float radius;
    float angle;

};

#endif
