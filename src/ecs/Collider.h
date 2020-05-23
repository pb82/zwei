#ifndef ZWEI_COLLIDER_H
#define ZWEI_COLLIDER_H

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "Component.h"
#include "Transform.h"
#include "../alg/Direction.h"

enum ColliderTag {
    CT_PLAYER,
    CT_ENEMY,
    CT_WALL,
    CT_PROJECTILE,
    CT_TRIGGER
};

class Collider : public Component {
public:
    Collider(Entity &parent, std::shared_ptr<Transform> tracked, ColliderTag tag,
             float dx = 0.0f, float dy = 0.0f);

    SDL_Rect boundingBox;

    void update(float dt) override;

    void collide(std::shared_ptr<Collider> other, SDL_Rect &intersection);

    ColliderTag tag;

    bool checked = false;

private:

    void updateBoundingBox();

    Direction inRelationTo(std::shared_ptr<Collider> other);

    std::shared_ptr<Transform> tracked;

    float dx;
    float dy;
};


#endif
