#ifndef ZWEI_COLLIDER_H
#define ZWEI_COLLIDER_H

#include <memory>

#include <SDL2/SDL.h>

#include "Component.h"
#include "Transform.h"

class Collider : public Component {
public:
    Collider(Entity &parent, std::shared_ptr<Transform> tracked,
             float dx = 0.0f, float dy = 0.0f);

    SDL_Rect boundingBox;

    void update(float dt) override;

private:

    void updateBoundingBox();

    std::shared_ptr<Transform> tracked;

    float dx;
    float dy;
};


#endif
