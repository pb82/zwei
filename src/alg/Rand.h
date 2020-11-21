#ifndef ZWEI_RAND_H
#define ZWEI_RAND_H

#include <cstdlib>

float getRandomFloat(float max) {
    return (float) rand() / (float) (RAND_MAX / max);
}

#endif //ZWEI_RAND_H
