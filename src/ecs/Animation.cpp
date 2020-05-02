#include "Animation.h"

Animation::Animation(Entity &parent, float speed, bool repeat)
        : Component(parent), speed(speed), repeat(repeat) {}

void Animation::addAnimationFrame(int frame, Direction d) {
    if (frames.find(d) == frames.end()) {
        frames.emplace(d, std::vector<int>());
    }
    frames.at(d).push_back(frame);
}

int Animation::getCurrentFrame(Direction d) {
    return frames[d].at(currentIndex);
}

void Animation::update(float dt) {
    // Animations can be one offs and not repeat after reaching
    // their last frame
    if (!repeat && currentIndex == (frames.size() - 1)) {
        return;
    }

    animationCount += dt;
    currentIndex = long(animationCount / speed) % frames.size();
}