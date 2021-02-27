#include "ASSETS/Assets.h"

#include "../src/Gfx.h"

Texture::~Texture() {
    SDL_DestroyTexture(this->mem);
}

SDLFont::~SDLFont() {
    TTF_CloseFont(this->mem);
}

void Assets::addFont(Asset id, EmbeddedAsset &asset) {
    fonts.emplace(id, &asset);

    auto raw = asset.raw();
    SDL_RWops *stream = SDL_RWFromConstMem(std::get<0>(raw), std::get<1>(raw));
    TTF_Font *font = TTF_OpenFontRW(stream, 1, 32);
    auto storage = std::make_shared<SDLFont>(font);
    ttf_fonts.emplace(id, storage);
}

void Assets::addTexture(Asset id, EmbeddedAsset &asset) {
    auto raw = asset.raw();
    SDL_RWops *stream = SDL_RWFromConstMem(std::get<0>(raw), std::get<1>(raw));
    SDL_Texture *texture = IMG_LoadTexture_RW(Gfx_Renderer, stream, 1);

    auto storage = std::make_shared<Texture>(texture);
    SDL_QueryTexture(texture, nullptr, nullptr, &storage->w, &storage->h);
    textures.emplace(id, storage);
}

void Assets::addTexture(Asset id, const char *file) {
    auto *texture = IMG_LoadTexture(Gfx_Renderer, file);
    auto storage = std::make_shared<Texture>(texture);
    SDL_QueryTexture(texture, nullptr, nullptr, &storage->w, &storage->h);
    textures.emplace(id, storage);
}

void *Assets::getFont(Asset id) {
    auto font = fonts.find(id);
    if (font == fonts.end()) {
        return nullptr;
    }

    auto raw = font->second->raw();
    return std::get<0>(raw);
}

std::shared_ptr<SDLFont> Assets::getSDLFont(Asset id) {
    auto font = ttf_fonts.find(id);
    if (font == ttf_fonts.end()) {
        return nullptr;
    }
    return font->second;
}

std::shared_ptr<Texture> Assets::getTexture(Asset id) {
    auto texture = textures.find(id);
    if (texture == textures.end()) {
        return nullptr;
    }
    return texture->second;
}
