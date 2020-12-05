#include "Twilight.h"

Twilight::Twilight(Position *tracked) : Filter(), tracked(tracked) {}

void Twilight::render(SDL_Texture *t, const Position *r) {
    float d = r->distance(*tracked);
    if (d <= 1) d = 1;

    SDL_SetTextureColorMod(t, 255 / d, 255 / d, 255 / d);
}