#include "Animation.h"

Animation::Animation(Entity &parent, float speed, bool repeat)
        : Component(parent), speed(speed), repeat(repeat) {}

void Animation::addFrameForDirection(Direction d, int frame) {
    if (frames.find(d) == frames.end()) {
        frames.emplace(d, std::vector<int>());
    }
    frames.at(d).push_back(frame);
}

void Animation::addAnimationFrame(int a) {
    addAnimationFrame(a, a, a, a);
}

void Animation::addAnimationFrame(int n, int s, int e, int w) {
    addFrameForDirection(N, n);
    addFrameForDirection(S, s);
    addFrameForDirection(E, e);
    addFrameForDirection(W, w);
    frameCount++;
}

int Animation::getCurrentFrame(Direction d) {
    return frames[d].at(currentIndex);
}

void Animation::update(float dt) {
    if (paused) return;

    // Animations can be one offs and not repeat after reaching
    // their last frame
    if (!repeat && currentIndex == (frameCount - 1)) {
        return;
    }

    animationCount += dt;
    currentIndex = long(animationCount / speed) % frameCount;
}