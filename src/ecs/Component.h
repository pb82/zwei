#ifndef ZWEI_COMPONENT_H
#define ZWEI_COMPONENT_H

class Entity;

class Component {
public:
    Component(const Entity &parent) : parent(parent) {}

    virtual ~Component() {}

    virtual void update() {}

    virtual void render() {}

private:
    const Entity &parent;
};

#endif
