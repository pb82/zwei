#ifndef ZWEI_MODIFIER_H
#define ZWEI_MODIFIER_H

#include "../Entity.h"

enum ModifierType {
    CIRCLE_OF_LIGHT
};

class Modifier {
public:

    Modifier(ModifierType type, float lifetime) : lifetime(lifetime), type(type) {}

    bool running() {
        return lifetime > 0;
    }

    virtual ~Modifier() {}

    virtual void update(float dt) {
        lifetime -= dt;
    }

    virtual float modify(float in) = 0;

    ModifierType type;

    float lifetime = 0.0f;

};

#endif //ZWEI_MODIFIER_H
