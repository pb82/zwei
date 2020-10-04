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

void Animation::addMixinFrame(int a) {
    addMixinFrame(a, a, a, a);
}

void Animation::addMixinFrame(int n, int s, int e, int w) {
    x[N] = n;
    x[S] = n;
    x[E] = n;
    x[W] = n;
}

int Animation::getCurrentFrame(Direction d) {
    return frames[d].at(currentIndex);
}

void Animation::update(float dt) {
    if (paused && currentIndex == 0 && x.empty()) return;

    // Animations can be one offs and not repeat after reaching
    // their last frame
    if (!repeat && currentIndex == (frameCount - 1)) {
        return;
    }

    if (!x.empty()) {
        return;
    }

    animationCount += dt;
    currentIndex = long(animationCount / speed) % frameCount;
}