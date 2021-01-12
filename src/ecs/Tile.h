#ifndef ZWEI_TILE_H
#define ZWEI_TILE_H

#include <vector>

#include <ASSETS/Assets.h>

#include "Component.h"

class Tile : public Component {
public:
    Tile(Entity &parent, Asset id);

    void render() override;

private:
    void pick(SDL_Rect &source);

    Asset id;

    std::shared_ptr<Texture> texture;
};

#endif
