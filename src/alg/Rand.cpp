#include "Rand.h"

float Rnd::getRandomFloat(float max) {
    return (float) rand() / (float) (RAND_MAX / max);
}

int Rnd::getRandomRange(int from, int to) {
    return from + (std::rand() % (to - from + 1));
}
