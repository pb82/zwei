#ifndef ZWEI_SPRITE_H
#define ZWEI_SPRITE_H

#include <ASSETS/Assets.h>

#include <queue>

#include "Component.h"
#include "filters/Filter.h"

#include "../alg/Vector.h"

class Sprite : public Component {
public:
    Sprite(Entity &parent, Asset id);

    void render() override;

    void update(float dt) override;

    void addFilter(std::shared_ptr<Filter> f);

private:
    void pick(SDL_Rect &source);

    std::queue<std::shared_ptr<Filter>> filters;

    Asset assetId;
};


#endif
