#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

#include "../in/Input.h"

enum LayerType {
    BACKGROUND = 0,
    FLOOR = 1,
    WALLS = 2,
    ITEMS = 3,
    OBJECTS = 4,
    FOREGROUND = 5,
    UI = 6
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
