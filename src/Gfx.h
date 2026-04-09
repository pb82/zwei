#ifndef ZWEI_GFX_H
#define ZWEI_GFX_H

#include <SDL.h>

#define Gfx_Renderer Gfx::instance().renderer
#define Gfx_Window Gfx::instance().window
#define Gfx_Tile_Size Gfx::instance().tileSize

#define PIXELFONT_WIDTH 7
#define PIXELFONT_HEIGHT 9

// Shared SDL and GL structures
class Gfx {
public:
    static Gfx &instance() {
        static Gfx instance;
        return instance;
    }

    Gfx(Gfx const &) = delete;

    void operator=(Gfx const &) = delete;

    static void pick(SDL_Rect &source, int tileId, int textureWidth);

    static void pickText(SDL_Rect &source, int tileId, int textureWidth);

    void shutdown();

    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

    unsigned int tileSize = 0;

private:
    Gfx() {}

    ~Gfx();
};

#endif
