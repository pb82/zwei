#ifndef ZWEI_SPRITE_H
#define ZWEI_SPRITE_H

#include <ASSETS/Assets.h>

#include "Component.h"
#include "../alg/Vector.h"

class Sprite : public Component {
public:
    Sprite(Entity &parent, Asset id, float angle);

    void render() override;

    void update(float dt) override;

private:
    void pick(SDL_Rect &source);

    Asset assetId;
    Vector trajectory;
};


#endif
