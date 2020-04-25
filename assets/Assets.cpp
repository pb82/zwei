#include "ASSETS/Assets.h"

#include "../src/Gfx.h"

Assets::~Assets() {
    for (auto texture: textures) {
        SDL_DestroyTexture(texture.second);
    }
}

void Assets::addFont(Asset id, EmbeddedAsset &asset) {
    fonts[id] = &asset;
}

void Assets::addTexture(Asset id, EmbeddedAsset &asset) {
    auto raw = asset.raw();
    SDL_RWops *stream = SDL_RWFromConstMem(std::get<0>(raw), std::get<1>(raw));
    SDL_Texture *texture = IMG_LoadTexture_RW(Gfx_Renderer, stream, 1);
    textures[id] = texture;
}

void *Assets::getFont(Asset id) {
    auto font = fonts.find(id);
    if (font == fonts.end()) {
        return nullptr;
    }

    auto raw = font->second->raw();
    return std::get<0>(raw);
}

SDL_Texture *Assets::getTexture(Asset id) {
    auto texture = textures.find(id);
    if (texture == textures.end()) {
        return nullptr;
    }
    return texture->second;
}