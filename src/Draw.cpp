#include "Draw.h"

#include "Gfx.h"

void Draw::draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target) {
    SDL_RenderCopy(Gfx_Renderer, texture, &source, &target);
}

void Draw::line(Color &c, int x1, int y1, int x2, int y2) {
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(Gfx_Renderer, x1, y1, x2, y2);
}