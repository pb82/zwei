#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

class Entity;

class Component {
public:
    Component(Entity &parent) : parent(parent) {}

    virtual ~Component() {}

    virtual void update(float dt) {}

    virtual void render() {}

protected:
    Entity &parent;
};

#endif
