#ifndef ZWEI_GFX_H
#define ZWEI_GFX_H

#include <SDL.h>

#define Gfx_GL_Context Gfx::instance().glContext
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

    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    SDL_Window *window;

    unsigned int tileSize;

private:
    Gfx() {}

    ~Gfx();
};

#endif
