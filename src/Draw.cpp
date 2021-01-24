#include "Draw.h"

#include "Gfx.h"

void Draw::draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Gfx_Renderer, texture, &source, &target, 0.0f, nullptr, flip);
}

void Draw::line(Color &c, int x1, int y1, int x2, int y2) {
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(Gfx_Renderer, x1, y1, x2, y2);
}

void Draw::rect(Color &c, SDL_Rect &rect) {
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(Gfx_Renderer, &rect);
}