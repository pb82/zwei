#ifndef ZWEI_GFX_H
#define ZWEI_GFX_H

#include <SDL2/SDL.h>

#define Gfx_GL_Context Gfx::instance().glContext
#define Gfx_Renderer Gfx::instance().renderer
#define Gfx_Window Gfx::instance().window

// Shared SDL and GL structures
class Gfx {
public:
    static Gfx &instance() {
        static Gfx instance;
        return instance;
    }

    Gfx(Gfx const &) = delete;

    void operator=(Gfx const &) = delete;

    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    SDL_Window *window;

private:
    Gfx() {}

    ~Gfx();
};

#endif
