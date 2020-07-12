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

    void collide(std::shared_ptr<Collider> other);

    ColliderTag tag;

    std::shared_ptr<Transform> tracked;

private:

    // Stop the motion of the parent of the given collider
    static void stop(const Collider& c);

    void notify(const Collider& c);

    void updateBoundingBox();

    void collideWall(std::shared_ptr<Collider> other, float bounce);

    void collideEnemy(std::shared_ptr<Collider> other);

    float dx;
    float dy;
};


#endif
