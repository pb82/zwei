#ifndef ZWEI_TILE_H
#define ZWEI_TILE_H

#include <ASSETS/Assets.h>

#include "Component.h"

class Tile : public Component {
public:
    Tile(Entity &parent, Asset id, int pos) : Component(parent), id(id), pos(pos) {}

    void render() override;

private:
    void pick(SDL_Rect &source);

    int pos;
    Asset id;
};

#endif
