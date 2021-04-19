#ifndef ZWEI_MODIFIER_H
#define ZWEI_MODIFIER_H

#include "../Entity.h"

enum ModifierType {
    CIRCLE_OF_LIGHT
};

class Modifier {
public:

    Modifier(ModifierType type, float lifetime, int tile) : lifetime(lifetime), max(lifetime), type(type), tile(tile) {}

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

    float max = 0.0f;

    int tile = 0;

};

#endif //ZWEI_MODIFIER_H
