#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

#include "../in/Input.h"

enum LayerType {
    BACKGROUND = 0,
    FLOOR = 1,
    WALLS = 2,
    ROOF = 3,
    ITEMS = 4,
    OBJECTS = 5,
    FOREGROUND = 6,
    UI = 7,
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
