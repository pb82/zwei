#ifndef ZWEI_SKELETON_H
#define ZWEI_SKELETON_H

#include "Mind.h"

class Skeleton : public Mind {
public:

    Skeleton(Entity &parent);

    ~Skeleton();

    int delay() override;

    void plan(float dt) override;

    bool activate() override;

    void render() override;

private:

    std::vector<Position> route;

    Position goal;

    // You need to get closer than 3 tiles for it to activate
    float activationRange = 10.0f;

};

#endif
