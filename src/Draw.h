#ifndef ZWEI_DRAW_H
#define ZWEI_DRAW_H

#include <stack>

#include <SDL.h>

#include "alg/Color.h"

struct TextureAlpha {
    SDL_Texture *texture;
    uint8_t alpha;
};

class Draw {
public:
    static Draw &instance() {
        static Draw instance;
        return instance;
    }

    Draw(Draw const &) = delete;

    void operator=(Draw const &) = delete;

    void draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target, float angle = 0.0f,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

    void draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target, SDL_Color color, float angle = 0.0f,
              SDL_RendererFlip flip = SDL_FLIP_NONE);

    void line(Color &c, int x1, int y1, int x2, int y2);

    void rect(Color &c, SDL_Rect &rect);

    void box(Color &c, SDL_Rect &rect);

    void box(Color &c, SDL_Rect &rect, uint8_t a);

    void pushColor();

    void popColor();

    void pushAlpha(SDL_Texture *texture);

    void popAlpha();

private:
    Draw() {}

    std::stack<SDL_Color> colors;
    std::stack<TextureAlpha> alphas;
};

#endif
