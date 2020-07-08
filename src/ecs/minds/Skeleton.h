#ifndef ZWEI_SKELETON_H
#define ZWEI_SKELETON_H

#include "Mind.h"

class Skeleton : public Mind {
public:

    Skeleton(Entity &parent);

    int delay() override;

    void plan(float dt) override;

    void collide(std::shared_ptr<Collider> other) override;

};



#endif
