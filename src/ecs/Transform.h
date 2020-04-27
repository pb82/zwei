#ifndef ZWEI_TRANSFORM_H
#define ZWEI_TRANSFORM_H

#include "../Rt.h"
#include "../alg/Position.h"

#include "Component.h"

class Transform : public Component {
public:
    Transform(Entity &parent, float x, float y)
            : Component(parent), p(x, y) {}

    Position p;
};

#endif
