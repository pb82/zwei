#ifndef ZWEI_COL_H
#define ZWEI_COL_H

#include <vector>
#include <memory>

#include "ecs/Collider.h"

struct ColliderGroup {
public:
    ColliderGroup(std::shared_ptr<Collider> subject) : subject(subject) {}

    void collidesWith(std::shared_ptr<Collider> colliding);

    int size = 0;

    std::shared_ptr<Collider> subject;

    std::unordered_map<ColliderTag, std::vector<std::shared_ptr<Collider>>> involved;
};

class Col {
public:

    static void collide(float dt);

};

#endif
