#ifndef ZWEI_DRAW_H
#define ZWEI_DRAW_H

#include <SDL2/SDL.h>

class Draw {
public:
    static Draw &instance() {
        static Draw instance;
        return instance;
    }

    Draw(Draw const &) = delete;

    void operator=(Draw const &) = delete;

    void draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target);

private:
    Draw() {}
};

#endif
