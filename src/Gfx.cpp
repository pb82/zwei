#include "Gfx.h"

#include <SDL_image.h>

void Gfx::pick(SDL_Rect &source, int tileId, int textureWidth) {
    source.x = (tileId * Gfx_Tile_Size) % textureWidth;
    source.y = (tileId * Gfx_Tile_Size) / textureWidth * Gfx_Tile_Size;
    source.w = Gfx_Tile_Size;
    source.h = Gfx_Tile_Size;
}

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