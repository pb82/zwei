#include "Tan.h"

Tan::Tan() {}

void Tan::render(SDL_Texture *t, const Position *) {
    SDL_SetTextureColorMod(t, 0, 192, 255);
}