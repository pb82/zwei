#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

#include "../in/Input.h"

enum LayerType {
    BACKGROUND = 0,
    FLOOR,
    WALLS,
    OBJECTS,
    FOREGROUND,
    UI
};

class Entity;

class Component {
public:
    Component(Entity &parent) : parent(parent) {}

    virtual ~Component() {}

    virtual void update(float dt) {}

    virtual void render() {}

    virtual void key(GameKeyEvent &key) {}

protected:
    Entity &parent;
};

#endif
