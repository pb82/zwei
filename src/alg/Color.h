#ifndef ZWEI_COLOR_H
#define ZWEI_COLOR_H

#include <cstdint>

struct Color {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

extern Color color_Red;
extern Color color_White;
extern Color color_Blue;

extern Color color_Good;
extern Color color_Medium;
extern Color color_Bad;

extern Color color_EnemyGood;
extern Color color_EnemyMedium;
extern Color color_EnemyBad;

#endif
