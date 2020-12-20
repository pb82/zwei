#ifndef ZWEI_RAND_H
#define ZWEI_RAND_H

#include <cstdlib>

struct Rnd {
    static float getRandomFloat(float max);

    static int getRandomRange(int from, int to);
};

#endif //ZWEI_RAND_H
