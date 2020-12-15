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
    auto f = MixinFrame();
    f.frames[N] = n;
    f.frames[S] = s;
    f.frames[E] = e;
    f.frames[W] = w;
    f.duration = 100;
    mixinFrames.push(f);
}

void Animation::addAttackFrame(int n, int s, int e, int w, float duration) {
    auto f = MixinFrame();
    f.frames[N] = n;
    f.frames[S] = s;
    f.frames[E] = e;
    f.frames[W] = w;
    f.duration = duration;
    attackFrames.push_back(f);
}

void Animation::queueAttackFrames() {
    for (auto &frame : attackFrames) {
        mixinFrames.push(frame);
    }
}

int Animation::getCurrentFrame(Direction d) {
    if (!mixinFrames.empty()) {
        auto f = mixinFrames.front();
        if (f.duration <= 0) {
            mixinFrames.pop();
        }
        return f.frames[d];
    }
    return frames[d].at(currentIndex);
}

void Animation::update(float dt) {
    if (paused && currentIndex == 0 && mixinFrames.empty()) return;

    // Animations can be one offs and not repeat after reaching
    // their last frame
    if (!repeat && currentIndex == (frameCount - 1)) {
        return;
    }

    if (!mixinFrames.empty()) {
        mixinFrames.front().duration -= dt;
    }

    animationCount += dt;
    currentIndex = long(animationCount / speed) % frameCount;
}

void Animation::start() {
    paused = false;
}

void Animation::stop() {
    paused = true;
}