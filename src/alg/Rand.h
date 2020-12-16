#ifndef ZWEI_RAND_H
#define ZWEI_RAND_H

#include <cstdlib>

float getRandomFloat(float max) {
    return (float) rand() / (float) (RAND_MAX / max);
}

int getRandomRange(int from, int to) {
    return from + (std::rand() % (to - from + 1));
}

#endif //ZWEI_RAND_H
