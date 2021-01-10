#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

#include "../in/Input.h"

enum LayerType {
    BACKGROUND = 0,
    FLOOR = 1,
    WALLS = 2,
    ROOF = 3,
    SKY = 4,
    ITEMS = 5,
    OBJECTS = 6,
    FOREGROUND = 7,
    UI = 8,
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
