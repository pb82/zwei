#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

#include <memory>

#include <JSON/value.h>

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

    virtual void render(uint8_t hints) {}

    virtual void key(GameKeyEvent &key) {}

    virtual void serialize(JSON::Value &to) {};

    virtual void deserialize(JSON::Value &from) {};

    virtual std::string name() { return "<todo>"; }

protected:
    Entity &parent;
};

#endif
