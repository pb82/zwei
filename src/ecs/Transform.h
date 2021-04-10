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

    std::string name() { return "transform"; }

    bool serialize(JSON::Value &to) {
        to["position"]["x"] = p.x;
        to["position"]["y"] = p.y;
        to["position"]["p"] = p.p;
        to["padding"]["bottom"] = padding.bottom;
        to["padding"]["top"] = padding.top;
        to["padding"]["left"] = padding.left;
        to["padding"]["right"] = padding.right;
        return true;
    }
};

#endif
