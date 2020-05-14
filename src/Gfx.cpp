#include "Gfx.h"

#include <SDL2/SDL_image.h>

Gfx::~Gfx() {
    SDL_GL_DeleteContext(glContext);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    IMG_Quit();
}