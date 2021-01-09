#ifndef ZWEI_COLLIDER_H
#define ZWEI_COLLIDER_H

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "Component.h"
#include "Transform.h"
#include "../alg/Direction.h"
#include "../alg/Vector.h"
#include "Projectile.h"

enum ColliderTag {
    CT_PLAYER,
    CT_ENEMY,
    CT_WALL,
    CT_PROJECTILE,
    CT_TRIGGER,
    CT_ITEM,
};

class Collider : public Component {
public:
    Collider(Entity &parent, std::shared_ptr<Transform> tracked, ColliderTag tag,
             Padding p = {0, 0, 0, 0});

    SDL_Rect boundingBox;

    void update(float dt) override;

    void stop();

    void pause(float duration);

    void disable();

    void collect();

    void defend(std::shared_ptr<Collider> projectile);

    void kick(std::shared_ptr<Collider> other);

    void setPadding(Padding p);

    ColliderTag tag;

    std::shared_ptr<Transform> tracked;

    bool checked;

    Padding padding;

private:

    // Stop the motion of the parent of the given collider
    static void stop(const Collider &c);

    void updateBoundingBox();

};

#endif
