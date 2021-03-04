#include "Draw.h"

#include "Gfx.h"

void Draw::draw(SDL_Texture *texture, SDL_Rect &source, SDL_Rect &target, float angle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Gfx_Renderer, texture, &source, &target, angle, nullptr, flip);
}

void Draw::line(Color &c, int x1, int y1, int x2, int y2) {
    pushColor();
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(Gfx_Renderer, x1, y1, x2, y2);
    popColor();
}

void Draw::rect(Color &c, SDL_Rect &rect) {
    pushColor();
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawRect(Gfx_Renderer, &rect);
    popColor();
}

void Draw::box(Color &c, SDL_Rect &rect) {
    pushColor();
    SDL_SetRenderDrawBlendMode(Gfx_Renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(Gfx_Renderer, &rect);
    popColor();
}

void Draw::pushColor() {
    SDL_Color c;
    SDL_GetRenderDrawColor(Gfx_Renderer, &c.r, &c.g, &c.b, &c.a);
    colors.push(c);
}

void Draw::popColor() {
    if (colors.empty()) return;
    SDL_Color c = colors.top();
    SDL_SetRenderDrawColor(Gfx_Renderer, c.r, c.g, c.b, c.a);
    colors.pop();
}