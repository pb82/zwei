#ifndef ZWEI_TRANSFORM_H
#define ZWEI_TRANSFORM_H

#include "../Rt.h"
#include "../alg/Position.h"

#include "Component.h"

class Transform : public Component {
public:
    Transform(Entity &parent, float x, float y, Padding padding = {0, 0, 0, 0})
            : Component(parent), p(x, y), padding(padding) {}

    Position p;
    Padding padding;
};

#endif
