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
    mixinFrames.emplace(n, s, e, w, 100);
}

void Animation::addAttackFrame(int n, int s, int e, int w, float duration, bool projectile) {
    if (projectile) {
        projectileAttackFrames.emplace_back(n, s, e, w, duration);
    } else {
        attackFrames.emplace_back(n, s, e, w, duration);
    }
}

void Animation::addStateFrame(int n, int speed) {
    stateFrames.emplace_back(n, speed);
}

void Animation::queueAttackFrames() {
    for (auto &frame: attackFrames) {
        mixinFrames.push(frame);
    }
}

void Animation::queueProjectileFrames() {
    for (auto &frame: projectileAttackFrames) {
        mixinFrames.push(frame);
    }
}

void Animation::queueStateFramesForward() {
    stateFramesEnd = true;
    int lastN = 0;
    int lastS = 0;
    int lastW = 0;
    int lastE = 0;
    for (int i = 1; i <= stateFrames.size() - 1; i++) {
        auto frame = this->stateFrames.at(i);
        mixinFrames.push(frame);
        lastN = frame.frames[N];
        lastS = frame.frames[S];
        lastE = frame.frames[E];
        lastW = frame.frames[W];
    }

    // Set final frame
    int size = this->frames.at(N).size() - 1;
    this->frames.at(N).at(size) = lastN;
    this->frames.at(S).at(size) = lastS;
    this->frames.at(E).at(size) = lastE;
    this->frames.at(W).at(size) = lastW;
}

void Animation::queueStateFramesBackward() {
    stateFramesEnd = false;
    int lastN = 0;
    int lastS = 0;
    int lastW = 0;
    int lastE = 0;
    for (int i = this->stateFrames.size() - 2; i >= 0; i--) {
        auto frame = this->stateFrames.at(i);
        mixinFrames.push(frame);
        lastN = frame.frames[N];
        lastS = frame.frames[S];
        lastE = frame.frames[E];
        lastW = frame.frames[W];
    }

    // Set final frame
    int size = this->frames.at(N).size() - 1;
    this->frames.at(N).at(size) = lastN;
    this->frames.at(S).at(size) = lastS;
    this->frames.at(E).at(size) = lastE;
    this->frames.at(W).at(size) = lastW;
}

void Animation::setFirstStateFrame() {
    if (stateFrames.empty()) return;
    stateFramesEnd = false;
    auto f = this->stateFrames.at(0);
    int size = this->frames.at(N).size() - 1;
    this->frames.at(N).at(size) = f.frames[N];
    this->frames.at(S).at(size) = f.frames[S];
    this->frames.at(E).at(size) = f.frames[E];
    this->frames.at(W).at(size) = f.frames[W];
}

void Animation::setLastStateFrame() {
    if (stateFrames.empty()) return;
    stateFramesEnd = true;
    auto f = this->stateFrames.at(this->stateFrames.size() - 1);
    int size = this->frames.at(N).size() - 1;
    this->frames.at(N).at(size) = f.frames[N];
    this->frames.at(S).at(size) = f.frames[S];
    this->frames.at(E).at(size) = f.frames[E];
    this->frames.at(W).at(size) = f.frames[W];
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