#ifndef ZWEI_TILE_H
#define ZWEI_TILE_H

#include <vector>

#include <ASSETS/Assets.h>

#include "Component.h"

class Tile : public Component {
public:
    Tile(Entity &parent, Asset id, int tile);

    void addAnimationFrame(int tile);

    void setAnimationSpeed(float ms);

    void render() override;

    void update(float dt) override;

private:
    void pick(SDL_Rect &source);

    std::vector<int> tiles;
    float animationSpeed;
    float animationCount = 0;
    int currentTile;
    Asset id;
};

#endif
