#ifndef ZWEI_ASSETS_H
#define ZWEI_ASSETS_H

#include <unordered_map>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "EMBEDDED/EmbeddedAsset.h"

enum Asset {
    FONT = 0,
    TILES,
    SPRITES,
    SOUND_PICKUP,
    MUSIC_1,
    MUSIC_GAMEOVER,
    UNDEFINED,
};

class Texture {
public:
    Texture(SDL_Texture *texture) : mem(texture) {}

    ~Texture();

    SDL_Texture *mem;
    int w;
    int h;
};

class Assets {
public:
    static Assets &instance() {
        static Assets instance;
        return instance;
    }

    Assets(Assets const &) = delete;

    void operator=(Assets const &) = delete;


    void addFont(Asset id, EmbeddedAsset &asset);

    void addTexture(Asset id, EmbeddedAsset &asset);

    void addTexture(Asset id, const char *file);

    void *getFont(Asset id);

    std::shared_ptr<Texture> getTexture(Asset id);

private:
    Assets() {}

    std::unordered_map<Asset, std::shared_ptr<Texture>> textures;
    std::unordered_map<Asset, EmbeddedAsset *> fonts;

};

#endif
