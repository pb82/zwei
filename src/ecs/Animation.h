#ifndef ZWEI_ANIMATION_H
#define ZWEI_ANIMATION_H

#include <unordered_map>
#include <vector>

#include "Component.h"

enum Direction {
    N = 1,
    S,
    E,
    W,
    A
};

class Animation : public Component {
public:
    Animation(Entity &parent, float speed, bool repeat);

    void update(float dt) override;

    void addAnimationFrame(int frame, Direction d = A);

    int getCurrentFrame(Direction d = A);

    float speed;

    bool repeat;

private:
    float animationCount = 0;

    int currentIndex = 0;

    std::unordered_map<Direction, std::vector<int>> frames;
};


#endif
