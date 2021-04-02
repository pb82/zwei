#ifndef ZWEI_COLOR_H
#define ZWEI_COLOR_H

#include <cstdint>
#include <SDL.h>

struct Color {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    SDL_Color toSdlColor() {
        return {r, g, b, a};
    }
};

extern Color color_Red;
extern Color color_White;
extern Color color_Blue;
extern Color color_Bubble;

extern Color color_Good;
extern Color color_Medium;
extern Color color_Bad;

extern Color color_EnemyGood;
extern Color color_EnemyMedium;
extern Color color_EnemyBad;
extern Color color_EnemyBlue;

extern Color color_Bubble;
extern Color color_Black;
extern Color color_Dark;

#endif
