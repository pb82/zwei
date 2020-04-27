#include "Draw.h"

#include "Gfx.h"

void Draw::draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target) {
    SDL_RenderCopy(Gfx_Renderer, texture, &source, &target);
}