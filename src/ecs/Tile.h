#ifndef ZWEI_TILE_H
#define ZWEI_TILE_H

#include <vector>

#include <ASSETS/Assets.h>

#include "Component.h"

class Tile : public Component {
public:
    Tile(Entity &parent, Asset id);

    void render(LayerType layer) override;

private:
    void pick(SDL_Rect &source);

    Asset id;
};

#endif
