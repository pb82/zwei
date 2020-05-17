#ifndef ZWEI_GFX_H
#define ZWEI_GFX_H

#include <SDL2/SDL.h>

#define Gfx_GL_Context Gfx::instance().glContext
#define Gfx_Renderer Gfx::instance().renderer
#define Gfx_Window Gfx::instance().window
#define Gfx_Tile_Size Gfx::instance().tileSize


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

    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    SDL_Window *window;

    unsigned int tileSize;

private:
    Gfx() {}

    ~Gfx();
};

#endif
