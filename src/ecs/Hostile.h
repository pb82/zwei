#ifndef ZWEI_HOSTILE_H
#define ZWEI_HOSTILE_H

#include "Component.h"

class Hostile : public Component {
public:
    Hostile(Entity &parent) : Component(parent) {}
};

#endif //ZWEI_HOSTILE_H
