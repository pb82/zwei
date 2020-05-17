#ifndef ZWEI_DRAW_H
#define ZWEI_DRAW_H

#include <SDL2/SDL.h>

#include "alg/Color.h"

class Draw {
public:
    static Draw &instance() {
        static Draw instance;
        return instance;
    }

    Draw(Draw const &) = delete;

    void operator=(Draw const &) = delete;

    void draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target);

    void line(Color &c, int x1, int y1, int x2, int y2);

    void rect(Color &c, SDL_Rect &r);

private:
    Draw() {}
};

#endif
