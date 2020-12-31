#include "ASSETS/Assets.h"

#include "../src/Gfx.h"

Texture::~Texture() {
    SDL_DestroyTexture(this->mem);
}

Sound::~Sound() {
    Mix_FreeChunk(this->sound);
}

Music::~Music() {
    Mix_FreeMusic(this->music);
}

void Assets::addFont(Asset id, EmbeddedAsset &asset) {
    fonts.emplace(id, &asset);
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

void Assets::addSound(Asset id, const char *file) {
    Mix_Chunk *s = Mix_LoadWAV(file);
    auto storage = std::make_shared<Sound>(s);
    sounds.emplace(id, storage);
}

void Assets::addMusic(Asset id, const char *file) {
    Mix_Music *s = Mix_LoadMUS(file);
    auto storage = std::make_shared<Music>(s);
    songs.emplace(id, storage);
}

void *Assets::getFont(Asset id) {
    auto font = fonts.find(id);
    if (font == fonts.end()) {
        return nullptr;
    }

    auto raw = font->second->raw();
    return std::get<0>(raw);
}

std::shared_ptr<Texture> Assets::getTexture(Asset id) {
    auto texture = textures.find(id);
    if (texture == textures.end()) {
        return nullptr;
    }
    return texture->second;
}

std::shared_ptr<Sound> Assets::getSound(Asset id) {
    auto sound = sounds.find(id);
    if (sound == sounds.end()) {
        return nullptr;
    }
    return sound->second;
}

std::shared_ptr<Music> Assets::getMusic(Asset id) {
    auto sound = songs.find(id);
    if (sound == songs.end()) {
        return nullptr;
    }
    return sound->second;
}