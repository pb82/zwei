#include "Halo.h"

Halo::Halo(float duration) : Filter(duration) {}

void Halo::render(SDL_Texture *t, const Position *) {
    float d = this->duration / this->initial;
    SDL_SetTextureColorMod(t, 255, 255 * d, 255 * d);
}