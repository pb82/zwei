#ifndef ZWEI_LAYERED_H
#define ZWEI_LAYERED_H

#include "Component.h"

class Layered : public Component {
public:
    Layered(Entity &parent, LayerType layer) : Component(parent), layer(layer) {}

    LayerType layer;
};

#endif
