#ifndef ZWEI_TILE_H
#define ZWEI_TILE_H

#include <vector>

#include <ASSETS/Assets.h>

#include "Component.h"
#include "../alg/Position.h"

class Tile : public Component {
public:
    Tile(Entity &parent, Asset id);

    void render(uint8_t) override;

private:
    void pick(SDL_Rect &source);

    Asset id;

    std::shared_ptr<Texture> texture;

    std::vector<Position> path;

};

#endif
