#include "Gfx.h"

#include <SDL_image.h>

void Gfx::pick(SDL_Rect &source, int tileId, int textureWidth) {
    source.x = (tileId * Gfx_Tile_Size) % textureWidth;
    source.y = (tileId * Gfx_Tile_Size) / textureWidth * Gfx_Tile_Size;
    source.w = Gfx_Tile_Size;
    source.h = Gfx_Tile_Size;
}

void Gfx::pickText(SDL_Rect &source, int tileId, int textureWidth) {
    source.x = (tileId * PIXELFONT_WIDTH) % textureWidth;
    source.y = 0;

    if (tileId >= 90) {
        source.y = 45;
    } else if (tileId >= 72) {
        source.y = 36;
    } else if (tileId >= 54) {
        source.y = 27;
    } else if (tileId >= 36) {
        source.y = 18;
    } else if (tileId >= 18) {
        source.y = 9;
    }

    source.w = PIXELFONT_WIDTH;
    source.h = PIXELFONT_HEIGHT;
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