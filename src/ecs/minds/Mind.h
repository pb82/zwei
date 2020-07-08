#ifndef ZWEI_MIND_H
#define ZWEI_MIND_H

#include "../Entity.h"
#include "../Collider.h"

class Mind {
public:
    Mind(Entity &parent) : parent(parent) {}

    virtual ~Mind() {}

    virtual void collide(std::shared_ptr<Collider> other) {}

    virtual void plan(float dt) {}

    virtual int delay() { return 1000; }

protected:

    Entity &parent;

};

#endif
